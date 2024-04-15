# IJIS_Code
# Repository README

This repository contains three codes and a folder with files for cover generation and reconstruction algorithms, as well as a database conversion.

## Codes

### `cover_gen.py`

This code implements the cover generation algorithm. It generates pure or mixed covers of the root node based on the provided inputs: the height of the initial tree and the number of identifiers present in the database. It randomly generates a configuration, calculates its cover, and reports the time taken.

### `mixed_recon.py`

This code reconstructs a mixed cover from a text file (`cover.txt`), which is output by `cover_gen.py`. It reports the time taken for reconstruction.

### `pure_recon.py`

This code reconstructs a pure cover and reports the time taken.

## Folder

### `Enron`

This folder contains the `main.c` file, which is a database conversion tool. It takes a database in the format specified in `DB.txt`, generates the converted database, and reports the number of original and reduced number of pairs for that database.

## Usage

Each code or tool within the repository has specific usage instructions detailed within its respective file. Refer to the comments and documentation within each file for guidance on how to execute them effectively.

## Disclaimer

This code is a proof-of-concept implementation. Use it with caution and understand that it may not be optimized for all scenarios. Before using it in a production environment, thoroughly review and test the code to ensure it meets your requirements.

## Contributions

Contributions to this repository are welcome. Feel free to fork the repository, make your changes, and submit a pull request. For major changes, please open an issue first to discuss the proposed changes.
