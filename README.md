# Implementasi dan Analisis Perbandingan Kinerja Algoritma Breadth-First Search untuk Pathfinding pada Grid Menggunakan Open Multi-Processing dan Open Computing Language

## Nama Penyusun Proyek
- Silvia (25032014006)

---

## Deskripsi Singkat Proyek dan Fitur
Proyek ini mengimplementasikan algoritma Breadth-First Search (BFS) untuk pencarian jalur (pathfinding) pada grid menggunakan tiga pendekatan komputasi yang berbeda, yaitu BFS secara sekuensial, OpenMP untuk paralelisme CPU multi-core, dan OpenCL untuk komputasi paralel pada GPU. Tujuan utama proyek adalah membandingkan kinerja ketiga metode tersebut melalui proses benchmarking berdasarkan waktu eksekusi, speedup, dan efficiency pada berbagai ukuran grid.

Fitur utama proyek ini meliputi:
- Implementasi BFS sekuensial sebagai baseline performa.
- Implementasi BFS paralel menggunakan OpenMP pada CPU multi-core.
- Implementasi BFS paralel menggunakan OpenCL pada GPU.
- Benchmarking waktu eksekusi untuk berbagai ukuran grid.
- Perhitungan nilai speedup dan efficiency.
- Penyimpanan hasil benchmark ke file results.txt.
- Penyimpanan lintasan hasil pencarian ke file path.txt.
- Visualisasi lintasan BFS menggunakan Python melalui library Matplotlib.
- Visualisasi grafik perbandingan performa, speedup, dan efficiency.

---

## Langkah-langkah Cara Menjalankan Proyek

1. **Persiapan File**
   Pastikan folder proyek berisi seluruh file program, yaitu:
   - main.cpp
   - grid.h
   - benchmark.h
   - sequential_bfs.h
   - openmp_bfs.h
   - opencl_bfs.h
   - visualize.ipynb

2. **Membuka Terminal**
   - Buka Command Prompt, PowerShell, atau terminal kemudian pastikan path sudah ke folder proyek yang berisi seluruh file program.
   Contoh:
   ```
   cd "C:\Users\USER\OneDrive\Documents\folder_with_your_files"
   ```

3. **Menyesuaikan Path vcpkg**
   Pastikan path berikut sesuai dengan lokasi instalasi vcpkg pada komputer yang digunakan:
   - Directory Include
   ```
   C:\Users\USER\vcpkg\installed\x64-windows\include
   ```
   - Directory Library
   ```
   C:\Users\USER\vcpkg\installed\x64-windows\lib
   ```
   Ganti USER dengan username Windows yang digunakan, dan juga pastikan lokasi folder include dan lib sesuai dengan lokasi instalasi vcpkg pada sistem.

4. **Melakukan Kompilasi Program**
   Jalankan perintah berikut:
   ```
   g++ main.cpp -DUSE_OPENCL -fopenmp -O2 -o bfs.exe -I"C:\Users\USER\vcpkg\installed\x64-windows\include" -L"C:\Users\USER\vcpkg\installed\x64-windows\lib" -lOpenCL
   ```

5. **Menjalankan Program**
   Setelah proses kompilasi berhasil, jalankan program menggunakan perintah:
   ```
   .\bfs.exe
   ```
   Program akan menjalankan:
   - BFS Sekuensial
   - BFS OpenMP
   - BFS OpenCL

6. **Melihat Hasil Output**
   Program akan menghasilkan dua file, yaitu:
   - (`results.txt`), yang berisi data waktu eksekusi, speedup, dan efficiency
   - (`path.txt`), yang berisi data grid dan path yang ditentukan oleh algoritma BFS

7. **Menjalankan Visualisasi Python**
   Buka file (`visualize.ipynb`) yang digunakan dalam proyek untuk menampilkan:
   - Animasi dan visualisasi path BFS
   - Grafik hasil running suatu grid
   - Grafik perbandingan waktu eksekusi, speedup, dan efficiency pada beberapa ukuran grid

---

## Link Video Presentasi Proyek
- https://www.youtube.com/watch?v=4D7Xtl_IRPQ
