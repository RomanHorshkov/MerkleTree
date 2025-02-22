#!/usr/bin/env python3
import os
import shutil

# List the folders to remove.
folders = [
    "data/transactions_256",
    "data/transactions_257",
    "data/transactions_4096",
    "data/transactions_4097",
]

for folder in folders:
    if os.path.exists(folder):
        shutil.rmtree(folder)
        print(f"Removed {folder}")

print("All specified folders removed.")

