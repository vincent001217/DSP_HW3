from io import open
f=open('Big5-ZhuYin.map','r',encoding='CP950')


mapping=dict()

while 1:
    line=f.readline()
    if not line:
        break

    b5,zy=line.split(' ')
    zy=zy.split('/')
    mapping[b5]=[b5]
    
    for each_item in zy:
        if each_item[0] not in mapping:
            mapping[each_item[0]]=[]
        if b5 not in mapping[each_item[0]]: 
            mapping[each_item[0]].append(b5)


f2=open('ZhuYin-Big5.map','wb')
for each_item in sorted(mapping):
    ls1=[each_item]

    for inner_item in mapping[each_item]:
        ls1.append(inner_item)
    
    value1=' '.join(ls1)
    line = "%s\n" % (value1)
    encodedline = line.encode('CP950')

    f2.write(encodedline)


