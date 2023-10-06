import matplotlib.pyplot as plt

x = [2, 4, 6, 8, 10, 12, 14, 16, 18, 20]

ssum = [7.2552, 7.1206, 7.2264, 7.0451, 6.7728, 7.0642, 7.2278, 7.1766, 7.1883, 7.0461]
msum = [3.7487, 2.8275, 1.4319, 1.1707, 1.0363, 1.0453, 1.1392, 1.1363, 1.1684, 1.1326]
msum_fp = [3.709, 1.9926, 1.3809, 1.1527, 1.0069, 0.9925, 1.1728, 1.183, 1.1718, 1.18]
msum_pt = [3.7057, 2.0112, 1.3744, 1.1516, 1.0534, 0.9387, 1.1378, 1.2826, 1.1292, 1.1468]
msum_as = [3.7253, 1.9708, 1.4124, 1.146, 1.0187, 1.1128, 1.1898, 1.2498, 1.2558, 1.203]

plt.xticks(x)
plt.xlabel("thread count")
plt.plot(x, ssum, label="single_sum")
plt.scatter(x, ssum)
plt.plot(x, msum, label="multi_sum")
plt.scatter(x, msum)
plt.plot(x, msum_fp, label="future-promise")
plt.scatter(x, msum_fp)
plt.plot(x, msum_pt, label="packaged_task")
plt.scatter(x, msum_pt)
plt.plot(x, msum_as, label="async")
plt.scatter(x, msum_as)
plt.legend(loc=0)
plt.show()
