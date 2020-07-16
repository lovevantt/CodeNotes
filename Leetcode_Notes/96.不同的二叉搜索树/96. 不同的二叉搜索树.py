def getnumsTree(n):
    f_x=[0]*(n+1)
    f_x[0]=1
    f_x[1]=1
    for i in range(2,n+1):
        for j in range(n):
            f_x[i]+=f_x[j]*f_x[i-1-j]
    return f_x[n]

for i in range(1,100+1):
    sums=getnumsTree(i)
    print(sums)
