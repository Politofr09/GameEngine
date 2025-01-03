import os
from PIL import Image
import sys

def create_cubemap(directory, output_filename="cubemap.png"):
    # Expected filenames for the cubemap faces
    face_files = {
        "posx": "posx.jpg",  # Positive X
        "negx": "negx.jpg",  # Negative X
        "posy": "posy.jpg",  # Positive Y
        "negy": "negy.jpg",  # Negative Y
        "posz": "posz.jpg",  # Positive Z
        "negz": "negz.jpg"   # Negative Z
    }

    # Load all face images
    images = {}
    for key, filename in face_files.items():
        filepath = os.path.join(directory, filename)
        if not os.path.exists(filepath):
            raise FileNotFoundError(f"Missing cubemap face: {filename} in {directory}")
        images[key] = Image.open(filepath)

    # Verify dimensions
    width, height = images["posx"].size
    for key, img in images.items():
        if img.size != (width, height):
            raise ValueError(f"All images must have the same dimensions. '{key}' has size {img.size}.")

    # Create output image with a 4x3 grid
    cubemap_width = width * 4
    cubemap_height = height * 3
    cubemap_image = Image.new("RGBA", (cubemap_width, cubemap_height))

    # Map images to cubemap layout
    face_positions = {
        "posx": (2 * width, height),  # +X
        "negx": (0 * width, height),  # -X
        "posy": (1 * width, 0 * height),  # +Y
        "negy": (1 * width, 2 * height),  # -Y
        "posz": (1 * width, height),  # +Z
        "negz": (3 * width, height)   # -Z
    }

    for key, (x, y) in face_positions.items():
        cubemap_image.paste(images[key], (x, y))

    # Save output cubemap in the same directory
    output_path = os.path.join(directory, output_filename)
    cubemap_image.save(output_path)
    print(f"Cubemap image saved to {output_path}")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python create_cubemap.py <directory> [output_filename]")
        sys.exit(1)

    input_directory = sys.argv[1]
    output_file = sys.argv[2] if len(sys.argv) > 2 else "cubemap.png"

    create_cubemap(input_directory, output_file)
