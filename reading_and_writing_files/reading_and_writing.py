# This is a sample Python script.

# Press ⌃R to execute it or replace it with your code.
# Press Double ⇧ to search everywhere for classes, files, tool windows, actions, and settings.


# Press the green button in the gutter to run the script.
if __name__ == '__main__':
    file_name: str = "myFirstFile.txt"
    file_object = open(file_name, "w+")

    for i in range(10):
        file_object.write("This is line %d\r\n" % (i + 1))

    file_object.close()

    file_object = open(file_name, "r")
    contents = file_object.read()
    print(contents)
