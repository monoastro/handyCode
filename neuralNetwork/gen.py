def gen(arr,step,n):
    res=[]
    for i in arr:
        res.append([x for x in [i+j*step for j in range(-n,n+1)]])
    return res



print(gen([5,5,5,5,5,5,5],3,2))
