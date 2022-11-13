import sys

def enumerate_groups(cigarstring):
    tmp_string = ''
    for elt in cigarstring:
        if elt.isnumeric():
            tmp_string = tmp_string + str(elt)
        else:
            yield (int(tmp_string), str(elt))
            tmp_string = ''

if __name__ == "__main__":

    if(len(sys.argv) > 1):
        threshold = float(sys.argv[1])
    else:
        threshold = 1.0

    for line in sys.stdin:
        if(line[0] == "@"):
            sys.stdout.write(line)
        else:
            cigarstr = line.split('\t')[5]
            groups = [(x,y) for (x,y) in enumerate_groups(cigarstr)]

            CIGAR = {"M":0, "=":0, "I":0, "X":0, "D":0, "H":0, "S":0, "P":0, "N":0}

            for (number,kind) in groups:
                CIGAR[kind] += number

            matches = CIGAR["M"] + CIGAR["="]
            seqlen = matches + CIGAR["I"] + CIGAR["S"] + CIGAR["H"] + CIGAR["N"]
            reflen = matches + CIGAR["D"] +CIGAR["N"]

            identity = (2 * matches) / (seqlen + reflen)
            if(identity >= threshold):
                sys.stdout.write(line)





