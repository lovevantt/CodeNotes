def twosums(numbers,target):
    i=0
    j=len(numbers)-1
    while i<j:
        if numbers[i]+numbers[j]<target:
            i=i+1
        elif numbers[i]+numbers[j]>target:
            j=j-1
        else:
            return [i+1,j+1]
numbers = [2, 7, 11, 15]
target = 9
print(twosums(numbers,target))