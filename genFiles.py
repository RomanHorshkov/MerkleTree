#!/usr/bin/env python3
import os

# Define the folders and the number of files to create in each.
folders = {
    "data/transactions_256": 256,
    "data/transactions_257": 257,
    "data/transactions_4096": 4096,
    "data/transactions_4097": 4097,
}

for folder, num_files in folders.items():
    os.makedirs(folder, exist_ok=True)
    for i in range(num_files):
        # Generate file names like file_0000.txt, file_0001.txt, etc.
        filename = os.path.join(folder, f"block_{i:04d}.txt")
        with open(filename, "w") as f:
            f.write(f"This is dummy content for file {i} in folder {folder}.\n")

print("Folders and dummy files generated successfully.")
