;; 设置默认的字体
(setq w32-charset-info-alist
    (cons '("gbk" w32-charset-gb2312 . 936) w32-charset-info-alist))

(setq default-frame-alist
      (append
       '((font . "fontset-gbk")) default-frame-alist))

(create-fontset-from-fontset-spec
  "-outline-Courier New-normal-r-normal-normal-20-97-96-96-c-*-fontset-gbk")
(set-fontset-font
 "fontset-default" nil
 "-outline-宋体-normal-r-normal-*-20-*-96-96-c-*-iso10646-1" nil 'prepend)
(set-fontset-font
 "fontset-gbk" 'kana
 "-outline-宋体-normal-r-normal-*-20-*-96-96-c-*-iso10646-1" nil 'prepend)
(set-fontset-font
 "fontset-gbk" 'han
 "-outline-宋体-normal-r-normal-*-20-*-96-96-c-*-iso10646-1" nil 'prepend)
(set-fontset-font
 "fontset-gbk" 'cjk-misc
 "-outline-宋体-normal-r-normal-*-20-*-96-96-c-*-iso10646-1" nil 'prepend)
(set-fontset-font
 "fontset-gbk" 'symbol
 "-outline-宋体-normal-r-normal-*-20-*-96-96-c-*-iso10646-1" nil 'prepend)
(set-default-font "fontset-gbk")