import matplotlib.pyplot as plt
fig = plt.figure()
for i in range(6):
    plt.subplot(2, 3, i+1)
    plt.tight_layout()
    plt.imshow([5, 6, 7, 8], cmap='gray', interpolation='none')
    plt.title("Ground Truth: {}".format([5, 6, 7, 8]))
    plt.xticks([])
    plt.yticks([])
plt.show()
