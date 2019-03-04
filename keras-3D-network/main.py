import SimpleITK as sitk
import os
import glob
import re

class ProcessingData:
  # Processing data and get the x_data and y_data

  def __init__(self, working_directory, volume_folder, mask_folder):
    self.working_directory = working_directory
    self.volume_location = os.path.join(working_directory, volume_folder)
    self.mask_location = os.path.join(working_directory, mask_folder)

  # Perform the operations to process the input
  def run(self):
    file_extension = '*.nii.gz'
    regex_pattern = r'\b([A-Z-]+)(\d+)'

    # iterate over all files into directory
    os.chdir(self.volume_location)
    for volume_filename in glob.glob(file_extension):
      try:
        list_filename = re.findall(regex_pattern, volume_filename)[0]  #the output is in the form [[head, tail]]
        if list_filename:
          #[0] should be LIDC-IDRI-, and [1] the number
          os.chdir(self.volume_location)
          print(volume_filename)
          os.chdir(self.mask_location)
          mask_filename = (list_filename[0] + list_filename[1] + '_GT1_Mask.nii.gz')
          print(mask_filename)
        else:
          print('Error in regex extraction:', volume_filename)
      except:
        print('Error in regex with \"', volume_filename, '\" filename')

class UNetNetwork:

  # initialiser
  def __init__(self):
    pass

'''
import keras
from keras.datasets import mnist
from keras.layers import Dense, Flatten
from keras.layers import Conv2D, Conv1D, MaxPooling2D, Dropout
from keras.models import Sequential
import matplotlib.pylab as plt
'''

# main function

data = ProcessingData('C:/Users/esmitt/Desktop/LIDC/', 'data/', 'mask/')
data.run()
'''
items = os.listdir('C:/Users/esmitt/Desktop/LIDC/data')

newlist = []
for names in items:
    if names.endswith(".nii.gz"):
        newlist.append(names)
print(newlist)
'''