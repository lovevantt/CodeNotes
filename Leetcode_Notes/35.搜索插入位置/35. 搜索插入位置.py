def searchInsert(nums, target) -> int:
    low=0
    high=len(nums)
    mid=0
    while(low<high):
        mid=low+(high-low)//2
        if(nums[mid]<target):
            low=mid+1
        elif(nums[mid]>=target):
            high=mid
    return low

nums=[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15]
n=searchInsert(nums,16)
print(n)