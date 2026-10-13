# Task 1 — Building a Balatro-Like Run

## A. Struktur Program

Program ini merupakan simulasi *run* sederhana yang terinspirasi dari sistem permainan seperti Balatro. Program berjalan selama 3 ronde, dan setiap ronde memiliki beberapa fase yang dilakukan dengan urutan yang sudah ditentukan.

Struktur file program:

```text
Task 1/
├── InputGenerator/
│   ├── InputGenerator.h
│   └── InputGenerator.cpp
├── ScoringRule/
│   ├── ScoringRule.h
│   └── ScoringRule.cpp
├── RewardRule/
│   ├── RewardRule.h
│   └── RewardRule.cpp
├── ShopSystem/
│   ├── ShopSystem.h
│   └── ShopSystem.cpp
├── RunSession/
│   ├── RunSession.h
│   └── RunSession.cpp
├── main.cpp
└── README.md
```

## B. Invariant Structure

Invariant adalah bagian dari sistem yang harus tetap sama. Pada program ini, invariant utamanya adalah urutan fase di dalam `RunSession`.

Urutannya adalah:

1. Generate input
2. Compute base score
3. Compute reward
4. Update money
5. Shop phase
6. Advance round

`RunSession` hanya bertugas mengatur urutan tersebut. Logic untuk membuat input, menghitung score, dan menghitung reward dipisahkan ke class masing-masing.

Urutan ini penting karena setiap fase menggunakan hasil dari fase sebelumnya. Misalnya, reward baru bisa dihitung setelah base score diketahui. Setelah reward didapatkan, barulah money diperbarui. Jika urutannya diubah, hasil dari satu fase bisa belum tersedia ketika fase berikutnya membutuhkannya.

## C. Mutable Parts

Mutable adalah bagian yang masih bisa diubah tanpa mengubah struktur utama program. Pada program ini, beberapa bagian yang mutable adalah:

**1. Input Generator**

Cara menghasilkan input dapat diubah. Pada awalnya digunakan `FixedInputGenerator`, kemudian diganti menjadi `RandomInputGenerator`.

**2. Scoring Rule**

Rumus untuk mendapatkan base score dapat diubah tanpa perlu mengubah urutan fase dalam `RunSession`.

**3. Reward Rule**

Jumlah reward juga dapat diubah. Pada modifikasi tugas ini, reward awalnya sama dengan base score, kemudian diubah menjadi `baseScore + 2`.

**4. Shop Content**

Isi atau penawaran dari shop dapat diganti tanpa memengaruhi urutan core loop.

Bagian-bagian tersebut termasuk mutable karena yang berubah adalah behavior-nya, sedangkan struktur utama program tetap sama.

## D. Modification 1 — Replace Input Generator

Pada versi awal, program menggunakan `FixedInputGenerator` yang selalu menghasilkan nilai `3`. Setelah dilakukan modifikasi, generator tersebut diganti dengan `RandomInputGenerator` yang menghasilkan nilai secara acak.

`RunSession` tidak perlu diubah karena `RunSession` menggunakan `IInputGenerator` sebagai interface. Jadi, `RunSession` tidak perlu mengetahui apakah input berasal dari `FixedInputGenerator` atau `RandomInputGenerator`. Selama class tersebut memiliki fungsi `Generate()`, keduanya dapat digunakan dengan cara yang sama.

## E. Modification 2 — Change Reward Logic

Pada versi awal, reward dihitung dengan:

```text
reward = baseScore
```

Kemudian reward diubah menjadi:

```text
reward = baseScore + 2
```

Perubahan ini cukup dilakukan pada implementasi `RewardRule`. `RunSession` tetap menggunakan proses yang sama, yaitu meminta reward dari `IRewardRule`.

Hal ini menunjukkan bahwa behavior dapat diubah tanpa mengubah core loop.

## F. Reflection

Invariant utama dalam program ini adalah urutan fase pada `RunSession`. Setiap ronde harus selalu dimulai dengan menghasilkan input, kemudian menghitung base score, menghitung reward, memperbarui money, menjalankan shop, dan terakhir berpindah ke ronde berikutnya. Struktur ini menjadi bagian yang tetap karena merupakan alur utama dari sebuah ronde.

Sementara itu, input generator, scoring rule, reward rule, dan shop termasuk bagian yang mutable. Behavior dari bagian tersebut dapat diubah sesuai kebutuhan tanpa harus mengubah urutan core loop.

Saat `FixedInputGenerator` diganti menjadi `RandomInputGenerator`, `RunSession` tidak perlu diubah karena `RunSession` hanya bergantung pada interface `IInputGenerator`. Kedua generator memiliki fungsi `Generate()` yang sama, sehingga `RunSession` dapat menggunakannya dengan cara yang sama meskipun cara menghasilkan input berbeda.

Jika scoring logic diletakkan langsung di dalam `RunSession`, maka `RunSession` akan menangani terlalu banyak hal. Ketika rumus scoring ingin diubah, `RunSession` juga harus ikut diubah. Hal ini membuat pemisahan antara struktur dan behavior menjadi kurang jelas dan kode lebih sulit dikembangkan.

Dengan memisahkan setiap tanggung jawab ke class masing-masing, kita dapat mengubah behavior game tanpa mengganggu struktur core loop.
