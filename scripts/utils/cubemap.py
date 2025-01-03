from PIL import Image
import sys

def create_cubemap(output_path):
    # Open all images
    images = {
        "posx": Image.open(output_path + "posx.jpg"),
        "negx": Image.open(output_path + "negx.jpg"),
        "posy": Image.open(output_path + "posy.jpg"),
        "negy": Image.open(output_path + "negy.jpg"),
        "posz": Image.open(output_path + "posz.jpg"),
        "negz": Image.open(output_path + "negz.jpg")
    }
    
    # Verify that all images have the same dimensions
    width, height = images["posx"].size
    for key, img in images.items():
        if img.size != (width, height):
            raise ValueError(f"All images must have the same dimensions. '{key}' has size {img.size}.")
    
    # Create a new image with 4x3 grid layout
    cubemap_width = width * 4
    cubemap_height = height * 3
    cubemap_image = Image.new("RGBA", (cubemap_width, cubemap_height))

    # Map images to cubemap layout
    # Layout:
    #       +----+----+----+----+
    #       |    | -Y |    |    |
    #       +----+----+----+----+
    #       | -X | +Z | +X | -Z |
    #       +----+----+----+----+
    #       |    | +Y |    |    |
    #       +----+----+----+----+
    face_positions = {
        "posx": (2 * width, height),      # +X
        "negx": (0 * width, height),      # -X
        "posy": (1 * width, 0 * height),  # +Y
        "negy": (1 * width, 2 * height),  # -Y
        "posz": (1 * width, height),      # +Z
        "negz": (3 * width, height)       # -Z
    }

    # Paste each face into the correct position
    for key, (x, y) in face_positions.items():
        cubemap_image.paste(images[key], (x, y))

    # Save the resulting cubemap
    cubemap_image.save(output_path + "cubemap.png")
    print(f"Cubemap image saved to {output_path}.cubemap.png")

if __name__ == "__main__":
    output_file = sys.argv[1]
    create_cubemap(output_file)
