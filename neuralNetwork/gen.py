def gen(arr,step,n):
    res=[]
    for i in arr:
        res.append([x for x in [i+j*step for j in range(-n,n+1)]])
    return res



ret = (gen([0.03, 0.98, 0.10, 1.00, 0.78, 0.90, 0.97, 0.34, 0.98], 0.01,2))
y = []
for i in range(5):
    x = []
    for j in range(9):
        x.append(float("{:.2f}".format(ret[j][i])))
    y.append(x)

for x in y:
    print(x, end=',\n')




