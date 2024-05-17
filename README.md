# Compile dapat dilakukan dengan hanya compile main filenya saja, juga bisa dirun lewat tombol run vscode karena langsung include c filenya, ga header filenya

## BEBERAPA NOTES
#### Buat main, udah ada fungsi buat milih algoritma apa yang mau dipake, seharusnya tinggal dipake aja

#### hati hati sama DFS karena cabang tree yang distancenya lebih sedikit ga diapus, jadi intinya ngeexpand adjacent matrix nya jadi tree, dengan traversal dfs, ini ngebuat memori yg dipake banyak bgt dan bikin laptop crash :v. Aku nyoba sampe 11 kota dia aman, tapi pas 12 kota udh bahaya

#### buat bfs, run pake yg bisa dirun tombol vscode aja, gara2 perlu nyiapin memori yg gede, filenya ga siap, jadinya bakal segmentation fault, atau ga coba file exenya pake data csv "data1.csv", run 2 kali, yang pertama biasanya gagal, yang ke2 berhasil, mungkin pas kedua dia udah nyiapin memori yg diperlukannya

#### kalo mau aman, pake yg bruteforce, ini pasti berhasil dan bener hasilnya, minusnya, buat yg jumlah kota gede, bakal lama bgt, tadi 12 kota masih bentar, pas 15 kota jgn ditunggu lagi :v

#### buat greedy, nama algoritmanya nearest neighbour algorithm, jdinya kemungkinan besar salah, dia cuma ngambil kota yg distancenya kecil, tapi plusnya runtimenya cepet bgt