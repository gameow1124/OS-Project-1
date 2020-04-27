# Operating Systems Project 1

學號: b05902012

系級: 資工四

姓名: 謝啟仁

## 設計

1. 使用雙核，一核用於scheduler，一核用於其他child，用sched_setaffinity來設定。

2. scheduler使用sched_setscheduler將要跑的child設定成SCHED_OTHER，將沒要跑的child設定成SCHED_IDLE。

3. 製作兩個syscall，一個是利用getnstimeofday來得到現在時間，一個是將字串寫入dmesg內。

4. scheduler會在child要結束的時候使用waitpid。
5. parent負責記錄process第一次搶到cpu並且執行的時間。
6. child structure內紀錄了start time, end time, 需要跑的時間, 已經跑的時間, 有沒有開始跑, 以及名字, pid等資訊。

   



## 核心版本

ubuntu linux 4.14.25

## 比較結果

理論上像是FIFO,SJF那種一個a跑完馬上另一個b就要接著跑的，應該要a的endtime 幾乎等於b的start time，

但實際上因為parent在控制著scheduler，所以會有一小段parent正在幫a收屍到啟動b的空窗期。

從FIFO_1_dmesg.txt就可以發現，會導致出現大約0.0000x秒的誤差。

​		

在PSJF這種每個utime都要檢查需不需要換人跑的時候，parent所計算的utime又會因為檢查而拉長，所以在跑PSJF的時候誤差又會大一點。



而有些誤差也有可能來自使用迴圈來等待執行時間，因此可以在TIMEMEASUREMENT_dmesg_txt之中發現，每次utime時間不可能會一模一樣。