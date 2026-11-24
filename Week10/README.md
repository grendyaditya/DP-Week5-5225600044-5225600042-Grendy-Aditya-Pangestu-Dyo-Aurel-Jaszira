# Praktikum Week 10 — Pengembangan Week 8

Week 10 ini merupakan **pengembangan langsung dari struktur dan alur Week 8**. Penamaan folder utama dipertahankan agar mudah dibandingkan dengan pekerjaan sebelumnya. Folder Week 8 asli tidak diubah.

## Struktur Folder

```text
Week10/
├── BlindSystem/
│   ├── BlindRule.cpp
│   └── BlindRule.h
├── ChosenHand/
│   └── ChosenHand.h
├── InputGenerator/
│   ├── InputGenerator.cpp
│   └── InputGenerator.h
├── JokerSystem/
│   ├── JokerSystem.cpp
│   └── JokerSystem.h
├── PokerHandChecker/
│   ├── Hand.h
│   ├── PokerHandChecker.cpp
│   └── PokerHandChecker.h
├── RewardRule/
│   ├── RewardRule.cpp
│   └── RewardRule.h
├── RunSession/
│   ├── RunSession.cpp
│   └── RunSession.h
├── ScoringRule/
│   ├── ScoringRule.cpp
│   └── ScoringRule.h
├── ShopSystem/
│   ├── ShopSystem.cpp
│   └── ShopSystem.h
├── tests/
│   ├── deck_test.cpp
│   ├── pattern_test.cpp
│   ├── scoring_test.cpp
│   └── selection_test.cpp
├── checker_test.cpp
├── main.cpp
├── run.sh
├── run.bat
└── README.md
```

## Alur Gameplay Week 10

1. `RandomInputGenerator` membuat satu standard deck berisi 52 kartu dan mengacaknya satu kali pada awal run.
2. Delapan kartu teratas dibagikan kepada pemain.
3. Kartu ditampilkan dengan posisi 1–8. Pemain memilih 1 sampai 5 kartu.
4. Input posisi dapat ditulis rapat seperti `415` (memilih posisi 4, 1, dan 5), atau dipisahkan spasi/koma seperti `4 1 5`.
5. `ChosenHand` menyimpan kartu yang dipilih. Urutan kartu di dalam `ChosenHand` mengikuti urutan input pemain.
6. `SimpleScoringRule` meneruskan kartu terpilih ke rantai `PokerHandChecker` untuk mendapatkan `HandRank`.
7. `ScoringRule` menghitung base Chips, Mult, card Chips, dan base score berdasarkan `HandRank`.
8. `JokerSubject` mengirim `HandPlayedEvent` kepada Observer. Efek Joker kemudian diterapkan melalui Decorator, mengikuti fondasi Week 8.
9. Kartu yang dimainkan dipindahkan ke discard pile. Sistem mengambil kartu pengganti sejumlah kartu yang dimainkan dari sisa deck, sehingga tangan kembali menjadi 8 kartu selama draw pile masih tersedia.
10. Jika draw pile habis, discard pile diacak kembali menjadi draw pile. Tidak ada kartu yang diduplikasi di antara holding, draw pile, dan discard pile.

Contoh: pemain mempunyai 8 kartu dan memasukkan `415`. Sistem membentuk `ChosenHand` dari kartu posisi ke-4, ke-1, dan ke-5. Tiga kartu itu diperiksa sebagai kombinasi poker, dinilai, diproses efek Joker, lalu diganti dengan tiga kartu dari deck.

## Batasan Evaluasi Hand

- High Card dapat dihitung dari 1–5 kartu.
- Pair dapat dihitung dari 2–5 kartu.
- Two Pair dapat dihitung dari 4–5 kartu.
- Three of a Kind dapat dihitung dari 3–5 kartu.
- Four of a Kind dapat dihitung dari 4–5 kartu.
- Straight, Flush, Full House, Straight Flush, dan Royal Flush memerlukan 5 kartu.
- Five of a Kind, Flush House, dan Flush Five tetap tersedia untuk pengujian checker, tetapi tidak akan muncul dari satu deck standar tanpa kartu duplikat.

## Tanggung Jawab Class

- `InputGenerator`: membuat/mengacak deck, deal awal 8 kartu, draw replacement, dan menyimpan discard pile.
- `ChosenHand`: menyimpan pilihan pemain sebanyak 1–5 kartu.
- `PokerHandChecker`: mengklasifikasikan kombinasi menjadi `HandRank` memakai Chain of Responsibility.
- `ScoringRule`: menentukan base Chips, Mult, dan card Chips berdasarkan hasil checker.
- `JokerSystem`: mengelola Observer dan Decorator untuk efek Joker.
- `RunSession`: mengatur urutan gameplay, input pemain, refill, Blind, reward, dan shop.
- `BlindSystem`, `RewardRule`, dan `ShopSystem`: mempertahankan dukungan gameplay dari Week 8.

## Build dan Run

Jalankan dari direktori `Week10`:

```bash
./run.sh
```

Atau compile manual menggunakan command satu baris:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp InputGenerator/InputGenerator.cpp ScoringRule/ScoringRule.cpp PokerHandChecker/PokerHandChecker.cpp RewardRule/RewardRule.cpp RunSession/RunSession.cpp ShopSystem/ShopSystem.cpp JokerSystem/JokerSystem.cpp BlindSystem/BlindRule.cpp -o Task1
```

Lalu jalankan:

```bash
./Task1
```

Windows: jalankan `run.bat` dari Command Prompt.

## Tests

```bash
g++ -std=c++17 checker_test.cpp ScoringRule/ScoringRule.cpp PokerHandChecker/PokerHandChecker.cpp -o checker_test && ./checker_test
```

```bash
g++ -std=c++17 tests/selection_test.cpp -o selection_test && ./selection_test
```

```bash
g++ -std=c++17 tests/deck_test.cpp InputGenerator/InputGenerator.cpp -o deck_test && ./deck_test
```

```bash
g++ -std=c++17 tests/pattern_test.cpp JokerSystem/JokerSystem.cpp -o pattern_test && ./pattern_test
```

```bash
g++ -std=c++17 tests/scoring_test.cpp ScoringRule/ScoringRule.cpp PokerHandChecker/PokerHandChecker.cpp -o scoring_test && ./scoring_test
```
