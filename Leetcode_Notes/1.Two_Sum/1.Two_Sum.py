def twoSum(nums, target):
    ii = jj = 0
    for i in nums:
        jj = ii+1
        for j in nums[(nums.index(i)+1):]:
            sum = i+j
            if sum == target:
                return [ii, jj]
            jj = jj+1
        ii = ii+1


nums = [4, 4]
target = 8
L = twoSum(nums, target)
print(L)
