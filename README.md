# CSAPP CacheLab

This repository contains my personal solutions and algorithm analysis for the CSAPP course.

---

##  Declaration & Copyright Notice
* Original Work: The source code provided in this repository is independently designed, written, and optimized by myself, accompanied by my algorithm analysis.
* Copyright & Academic Integrity: This repository is for personal learning and portfolio purposes only. Official lab handouts, test traces, and evaluation/grading scripts are excluded from this repository due to copyright and academic integrity policies.

---

##  File Descriptions

* **'csim.c'**: A cache simulator that mimics cache behavior. It reads memory access traces to compute hits, misses, and evictions based on customizable parameters ($s, E, b$) and implements the LRU (Least Recently Used) replacement policy.
* **'trans.c'**: An optimized matrix transposition implementation. It uses block-based algorithms and customized strategies to maximize cache locality and minimize miss rates for various matrix sizes ($32 \times 32$, $64 \times 64$, and non-square $60 \times 68$).
