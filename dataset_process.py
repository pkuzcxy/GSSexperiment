newFile = open("./lkml-reply/new-out.lkml-reply","w")
with open("./lkml-reply/out.lkml-reply","r") as f:
    lines= f.readlines()
    lines = lines[1:]
    lines.sort(key=lambda x:int(x.split()[3]))
    for line in lines:
        newFile.write(line)