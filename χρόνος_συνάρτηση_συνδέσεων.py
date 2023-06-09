import matplotlib.pyplot as plt


x = [78, 6594, 11850]
y = [0.000790, 199.834324, 987.199260]



plt.plot(x, y, marker='o', markersize=8, markerfacecolor='red')

# Set the tick labels for the x and y axes
plt.xticks(x)
plt.yticks(y)

plt.title("Διάγραμμα Χρόνου Εκτέλεσης Συνάρτηση Συνδέσεων")
plt.xlabel("Αριθμός Συνδέσεων")
plt.ylabel("Χρόνος σε second")

plt.show()
