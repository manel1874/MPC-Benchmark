input1 = [0,0,1,2,3,2,1,3,0,1,3,2]

filename = 'Bob_1_seq.txt'

def print_zero(filename):
    with open(filename, 'a+') as file_object:
        file_object.write("0\n")
        file_object.write("0\n")
        file_object.write("0\n")
        file_object.write("0\n")
        file_object.write("0\n")
        file_object.write("0\n")
        file_object.write("0\n")
        file_object.write("0\n")

def print_one(filename):
    with open(filename, 'a+') as file_object:
        file_object.write("1\n")
        file_object.write("0\n")
        file_object.write("0\n")
        file_object.write("0\n")
        file_object.write("0\n")
        file_object.write("0\n")
        file_object.write("0\n")
        file_object.write("0\n")

def print_two(filename):
    with open(filename, 'a+') as file_object:
        file_object.write("0\n")
        file_object.write("1\n")
        file_object.write("0\n")
        file_object.write("0\n")
        file_object.write("0\n")
        file_object.write("0\n")
        file_object.write("0\n")
        file_object.write("0\n")

def print_three(filename):
    with open(filename, 'a+') as file_object:
        file_object.write("1\n")
        file_object.write("1\n")
        file_object.write("0\n")
        file_object.write("0\n")
        file_object.write("0\n")
        file_object.write("0\n")
        file_object.write("0\n")
        file_object.write("0\n")

for elem in input1:
    if elem == 0:
        print_zero(filename)
    elif elem == 1:
        print_one(filename)
    elif elem == 2:
        print_two(filename)
    elif elem == 3:
        print_three(filename)



