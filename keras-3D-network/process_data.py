import os
import glob
import re

from SimpleITK import ReadImage, GetArrayFromImage
import numpy as np


class Metadata:
    """To handle the orogin, spacing, direction from the volume read using ITK"""

    def __init__(self, origin = None, spacing = None, direction = None):
        self.origin = origin
        self.spacing = spacing
        self.direction = direction

    def print(self):
        print("Origin: {0}; Spacing: {1}; Direction: {2}".format(self.origin, self.spacing, self.direction))


class ProcessingData:
    """Class to process the data and get the x_data and y_data.

        Process the input in a proper way to obtain a list of volume and mask data of lesions.
    """

    # Initializer of variables
    def __init__(self, working_directory, volume_folder, mask_folder):
        self._volume_location = os.path.join(working_directory, volume_folder)
        self._mask_location = os.path.join(working_directory, mask_folder)
        self.metadata = None
        self.list_data_volume = []  # Initialize empty list for volume data
        self.list_data_mask = []    # Initialize empty list for mask data

    # Read data in format Nifty
    # noinspection PyMethodMayBeStatic
    def read_nifty_data(self, filename):
        if os.path.exists(filename):
            temp_image = ReadImage(filename)
            temp_array = GetArrayFromImage(temp_image) # the coordinate system is changed from: (x,y,z) -> (z,y,x)
            return np.transpose(temp_array, (2, 1, 0))  # change the coordinate system as xyz
        else:   # Maybe, print some error too
            return np.empty()

    ''' 
    Perform the operations to process the input
    '''
    def run(self):
        file_extension = '*.nii.gz'
        regex_pattern = r'\b([A-Z-]+)(\d+)'

        # iterate over all files into directory, and read them
        os.chdir(self._volume_location)
        all_files = glob.glob(file_extension)
        index = 1
        for volume_filename in all_files:
            try:
                list_filename = re.findall(regex_pattern, volume_filename)[0]  # Output is in the form [[head, tail]]
                if list_filename:
                    # position [0] should be LIDC-IDRI-, and position [1] is the number
                    mask_filename = (list_filename[0].strip() + list_filename[1].strip() + '_GT1_Mask.nii.gz')

                    os.chdir(self._volume_location)
                    volume_data = self.read_nifty_data(volume_filename)
                    if len(volume_data) > 0:
                        self.list_data_volume.append(volume_data)

                    # reading the mask file
                    os.chdir(self._mask_location)
                    mask_data = self.read_nifty_data(mask_filename)
                    if len(mask_data) > 0:
                        self.list_data_mask.append(mask_data)

                    print("{0}/{1} ({2}, {3}) processed".format(index, len(all_files), volume_filename, mask_filename))
                    index = index + 1
                    '''self.metadata = Metadata(temp_volume_image.GetOrigin(), temp_volume_image.GetSpacing(), temp_volume_image.GetDirection())
                    self.metadata.print()
                    '''
                else:
                    print('Error in regex extraction:', volume_filename)
            except Exception as ex:
                template = "An exception of type {0} occurred. Arguments:\n{1!r}"
                message = template.format(type(ex).__name__, ex.args)
                print(message)
