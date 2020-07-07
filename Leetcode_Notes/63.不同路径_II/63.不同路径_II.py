#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import numpy
mm = 4  # m列
nn = 3  # n行
input_data = numpy.zeros((nn, mm), dtype=numpy.int16)
# 另一种列表初始化方法：obstacleGrid=[[0 for i in range(m)] for i in range(n)]
# 障碍物初始化：置1
input_data[1][1] = 1


def uniquePathsWithObstacles(obstacleGrid):
    # 假装不知道原数据的大小,获取输入数据的列数(m)和行数(n)
    m = obstacleGrid.shape[1]
    n = obstacleGrid.shape[0]
    dp = numpy.zeros((n, m), dtype=numpy.int16)
    if(obstacleGrid[0][0] == 1):
        return dp[0][0]
    for i in range(m):
        if obstacleGrid[0][i] == 0:
            dp[0][i] = 1
        else:
            break
    for j in range(n):
        if obstacleGrid[j][0] == 0:
            dp[j][0] = 1
        else:
            break
    for i in range(1, m):
        for j in range(1, n):
            if obstacleGrid[j][i] == 0:
                dp[j][i] = dp[j-1][i]+dp[j][i-1]
    return dp[j][i]


x = uniquePathsWithObstacles(input_data)
print(x)
