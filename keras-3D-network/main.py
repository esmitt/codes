from process_data import ProcessingData
import numpy as np

class UNetNetwork:

    # Initializer
    def __init__(self, list_data_volume, list_data_mask):
        self.list_data_volume = list_data_volume
        self.list_data_mask = list_data_mask
        [print(np.shape(x)) for x in self.list_data_volume]


# main function
data = ProcessingData('C:/Users/eramirez/Desktop/LIDC/', 'data/', 'mask/')
data.run()

network = UNetNetwork(data.list_data_volume, data.list_data_mask)