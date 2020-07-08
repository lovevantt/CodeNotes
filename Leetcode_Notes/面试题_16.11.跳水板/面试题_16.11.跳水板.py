def divingBoard(shorter,longer,k):
    value=set()
    result=[]
    if k==0:
        return []
    elif shorter==longer:
        return [shorter*k]
    for i in range(k+1):
        if((i*longer+(k-i)*shorter) not in value):
            value.add(i*longer+(k-i)*shorter)
            result.append(i*longer+(k-i)*shorter)
    #result=list(value)
    return result

l=divingBoard(1,2,0)
print(l)
l=divingBoard(2,2,2)
print(l)
l=divingBoard(2,168,56)
print(l)