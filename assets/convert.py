import os
import argparse
import re 
import cv2
import numpy as np

def get_image_files(image_path):
    # image_path is a directory path, list files in the directory
    if not os.path.isdir(image_path):
        raise ValueError(f"The specified path is not a directory: {image_path}")
    
    image_files = [f for f in os.listdir(image_path) if f.lower().endswith('.png')]
    image_files = sorted(image_files, key=lambda f: int(re.search(r'\d+', f).group()))

    return image_files

def files_to_c_arr(root, image_paths): 
    frames = "static const unsigned char eyes[][16][16][3] = {\n"
    for image_path in image_paths:

        img = cv2.imread(root+image_path, cv2.IMREAD_UNCHANGED)

        if img is None:
            raise FileNotFoundError(f"Image not found: {image_path}")
        pixels = np.array(img)
        frames += "    {\n"


    # If image has alpha channel
        b, g, r, a = cv2.split(img)
        alpha = a.astype(float) / 255.0

        # Blend with black background
        r = (r * alpha).astype(np.uint8)
        g = (g * alpha).astype(np.uint8)
        b = (b * alpha).astype(np.uint8)
        rgb_img = cv2.merge((r, g, b))

        # Build the formatted string
        rows = []
        for row in rgb_img:
            formatted_row = ",".join(f"{{0x{r:02X},0x{g:02X},0x{b:02X}}}" for r, g, b in row)
            rows.append(f"        {{{formatted_row}}},")

        # Join all rows with newlines
        frames += "\n".join(rows)
        frames += "\n    }, \n"
    frames += "};\n"

    return frames


def parse_arguments():
    parser = argparse.ArgumentParser(description="Convert a PNG image to an array of pixel values.")
    parser.add_argument('--image_path', type=str, help='Path to the PNG image file directory')
    parser.add_argument('--output', type=str, default='pixel_values.txt', help='Output file to save pixel values')
    return parser.parse_args()

args = parse_arguments()
image_path = args.image_path
output_file = args.output

print(f"Converting images in directory: {image_path}")
print(f"Converted file: {output_file}")

files = get_image_files(image_path)
out =  files_to_c_arr(image_path, files)

file = open(output_file, "w")
file.write(out)
file.close()

