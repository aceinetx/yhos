;;
;; yhOS
;; Copyright (c) aceinet
;; License: GPL-2.0
;;
format ELF

;; yhse header begin
;; db "YHSE", 0
;; dd _start
;; yhse header end

SYS_WRITEC = 0
SYS_WRITE = 1
SYS_GETS = 2
SYS_VFSWRITE = 3
SYS_VFSREAD = 4
SYS_VFSQUERY = 5
SYS_ALLOC = 6
SYS_FREE = 7
SYS_VFSBASE = 8
SYS_EXEARG = 9
SYS_VFSHANDLE = 10
SYS_ITOA = 11
SYS_ITOA16 = 12
SYS_GETCWD = 13
