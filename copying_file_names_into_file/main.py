# This is a sample Python script.

# Press ⌃R to execute it or replace it with your code.
# Press Double ⇧ to search everywhere for classes, files, tool windows, actions, and settings.
import os
import random
import string


def create_random_file_name():
    format_of_random_letters = string.ascii_letters

    random_file_name = ''.join(random.choice(format_of_random_letters) for _ in range(5))

    return random_file_name


# Creates files in the current workspace in the format "beginning_" + random characters + "_end.txt"
def create_file(num_files_to_create: int):
    for _ in range(num_files_to_create):
        file_name = "beginning_" + create_random_file_name() + "_" + create_random_file_name() + "_end.txt"
        new_file_being_made = open(file_name, "w+")
        new_file_being_made.close()


# Gets a list of all files in the workspace that contain beginning_ or _end
# @return a list of filenames (string)
def get_list_of_filenames():
    files_in_current_directory = os.listdir()
    remaining_unneeded_files_remain = True

    while remaining_unneeded_files_remain is True:
        found_a_filename_to_be_removed = False

        for fileIndex in range(len(files_in_current_directory)):
            current_filename_to_check = files_in_current_directory[fileIndex]
            if "beginning_" not in current_filename_to_check or "_end" not in current_filename_to_check:
                files_in_current_directory.pop(fileIndex)
                found_a_filename_to_be_removed = True
                break

        if found_a_filename_to_be_removed is not True:
            remaining_unneeded_files_remain = False

    return files_in_current_directory


# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    file_name_output: str = "file_being_written_to.txt"
    file_output = open(file_name_output, "w+")

    # call create_file if the files are not created yet
    # create_file(50)

    listOfFiles = get_list_of_filenames()

    for current_filename in listOfFiles:
        file_output.write(current_filename + "\r\n")

    file_output.close()
