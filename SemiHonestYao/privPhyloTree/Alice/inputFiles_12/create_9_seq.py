input1 = [1,2,2,0,0,1,3,2,1,3,1,1]

seq_number = "seq_8.txt"

filename = 'Party_0_' + seq_number

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


YaoConfig = """print_output = true
input_section = AES
circuit_type = NoFixedKey

# OS name is added automatically
[AES-Linux]
circuit_file = mainScapi.txt
input_file_party_1 = Inputs/"""+filename+"""
input_file_party_2 = Inputs/"""+filename

file_object = open("../YaoConfig_" + seq_number, "w")
file_object.write(YaoConfig)

file_object.close()


