import sys

filename = sys.argv[1]
nameArr = filename.split('.')
outName = ".".join(nameArr[0:-1]) + ".lc3"

def tokenizeLine(line):
    codeAndComment = line.split(';')
    comment = None
    if len(codeAndComment) > 2:
        comment = ';' +  " ".join(codeAndComment[1:])
    if len(codeAndComment) == 2:
        comment = ';' + codeAndComment[1]

    return (codeAndComment[0], comment)


def shouldTransform(line):
    if "NOR" in line: return True
    if "XNOR" in line: return True
    if "NAND" in line: return True
    if "LDBSE" in line: return True

    return False

def transformXNOR(instargs):
    transformed = "XOR {0}, {1}, {2}\n".format(instargs[0], instargs[1], instargs[2])
    transformed += "NOT {0}, {0}\n".format(instargs[0])
    return transformed

def transformNOR(instargs):
    transformed = "NOR {0}, {1}, {2}\n".format(instargs[0], instargs[1], instargs[2])
    transformed += "NOT {0}, {0}\n".format(instargs[0])
    return transformed

def transformNAND(instargs):
    transformed = "NAND {0}, {1}, {2}\n".format(instargs[0], instargs[1], instargs[2])
    transformed += "NOT {0}, {0}\n".format(instargs[0])
    return transformed

def transformLDBSE(instargs):
    transformed = "LDB {0}, {1}, {2}\n".format(instargs[0], instargs[1], instargs[2])
    transformed += "LSHFL {0}, 8\n".format(instargs[0])
    transformed += "RSHFA {0}, 8\n".format(instargs[0])
    return transformed

def getInstructionArguments(line):
    return line.split(" ")

def transformLine(line, comment):
    # addTab = False
    # if line[0] == ' ':
    #     print("TRUE")
    #     addTab = True
    line = line.strip()
    line = line.replace(',', '')
    args = getInstructionArguments(line)
    if args[0] == "XNOR":
        transformed = transformXNOR(args[1:])
    elif args[0] == "NOR":
        transformed = transformNOR(args[1:])
    elif args[0] == "NAND":
        transformed = transformNAND(args[1:])
    elif args[0] == "LDBSE":
        transformed = transformLDBSE(args[1:])

    # if addTab == True:
    #     transformed = '\t' + transformed

    if comment is not None:
        return comment + transformed
    else:
        return transformed

def run():
    with open(filename, "r") as inFile, open(outName, "w") as outFile:
        for line in inFile:
            instr, comment = tokenizeLine(line)
            # print((instr, comment))
            # outFile.write(str(tokenized))
            if shouldTransform(line):
                transformedLine = transformLine(instr, comment)
                print(transformedLine)
                outFile.write(transformedLine)
            else:
                print(line)
                outFile.write(line)
run()
