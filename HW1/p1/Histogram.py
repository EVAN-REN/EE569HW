import numpy as np
import matplotlib.pyplot as plt
import math

def read_raw_image(file_path, height, width):
    # read raw image
    with open(file_path, 'rb') as file:
        raw_data = np.fromfile(file, dtype=np.uint8)
    
    # convert shape
    image = raw_data.reshape((height, width))

    return image

def plot_histogram(image):
    # flat image
    flattened_image = image.flatten()

    # plot histogram
    y_values = [0] * 256
    for pixel_value in flattened_image:
        y_values[pixel_value] += 1
    
    x_values = list(range(256))
    plt.bar(x_values, y_values, color='blue')
    plt.title('Histogram of Original Image')
    plt.xlabel('Pixel Intensity')
    plt.ylabel('Number of Pixels')
    plt.xlim(0, 256)
    plt.ylim(0, 10000)
    plt.show()

def transfer_function_based(image):
    # flat image
    flattened_image = image.flatten()

    # Count the frequency of pixels of each grayscale value
    y_values = [0] * 256
    for pixel_value in flattened_image:
        y_values[pixel_value] += 1
    x_values = list(y_values)
    
    # Calculate the normalized probability histogram
    for i in range(256):
        x_values[i] /= len(flattened_image)
    
    # Calculate the CDF
    accumulate = 0.0
    for i in range(256):
        accumulate += x_values[i]
        x_values[i] = accumulate
    
    #  Create the mapping-table
    for i in range(256):
        x_values[i] = math.floor(x_values[i] * 255)

    # plot the histogram
    plt.bar(x_values, y_values, color='blue')
    plt.title('Transfer Function Based Histogram')
    plt.xlabel('Pixel Intensity')
    plt.ylabel('Number of Pixels')
    plt.xlim(0, 256)
    plt.ylim(0, 3000)
    plt.show()
    
def bucket_filling(image):
    # flat image
    flattened_image = image.flatten()

    y_values = [0] * 256
    x_values = list(range(256))
    totolPixels = len(flattened_image)
    binNumber = math.floor(totolPixels / 256)
    restPixel = totolPixels % 256
    for i in range(256):
        y_values[i] = binNumber
    for i in range(restPixel):
        y_values[i] += 1
    
    # plot the histogram
    plt.bar(x_values, y_values, color='blue')
    plt.title('Bucket Filling Histogram')
    plt.xlabel('Pixel Intensity')
    plt.ylabel('Number of Pixels')
    plt.xlim(0, 256)
    plt.ylim(0, 1200)
    plt.show()

    



raw_file_path = '/Users/ren/Documents/hw/EE569/HW1_materials/images/DimLight.raw'
image_height = 340
image_width = 596

image = read_raw_image(raw_file_path, image_height, image_width)
plot_histogram(image)
transfer_function_based(image)
bucket_filling(image)
