;; -*-mode:lisp-interaction-*- -*- coding: gbk-dos -*-

;; load path
(add-to-list 'load-path "~/emacs/")

;; color theme
(require 'color-theme)
(color-theme-initialize)
(color-theme-wombat)

(custom-set-variables
 ;; custom-set-variables was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 '(inverse-video t)
 '(python-indent 8)
 '(standard-indent 8))

(custom-set-faces
 ;; custom-set-faces was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 '(default ((t (:inherit nil :stipple nil :background "black" :foreground "green" :inverse-video nil :box nil :strike-through nil :overline nil :underline nil :slant normal :weight normal :height 150 :width normal :foundry "outline" :family "新宋体")))))

;(set-cursor-color "ivory3")
(set-mouse-color "white")

(setq w32-pass-lwindow-to-system nil 
      w32-pass-rwindow-to-system nil 
      w32-pass-apps-to-system nil 
      w32-lwindow-modifier 'super ;; Left Windows key 
      w32-rwindow-modifier 'super ;; Right Windows key 
      w32-apps-modifier 'hyper) ;; Menu key


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 外观显示 ;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;禁用启动画面
(setq inhibit-startup-message t)
;; *scratch* buffer的提示信息
(setq initial-scratch-message "")
;;尺寸
(setq initial-frame-alist '( (width . 80) (height . 25))) 
;;标题格式, "文件名  @  全路径文件名"
(setq frame-title-format '("%b@" buffer-file-name))
;;取消显示工具栏
(if (functionp 'tool-bar-mode) (tool-bar-mode nil)) 

;; 取消显示菜单栏
(if (functionp 'menu-bar-mode) (menu-bar-mode nil))
 
;;去掉滚动条, 鼠标滚轮代替
(if (functionp 'set-scroll-bar-mode) (set-scroll-bar-mode nil))
 
;;底栏显示列号
(setq column-number-mode t)

;;显示括号匹配
(show-paren-mode t)
(setq show-paren-style 'parenthesis)

 
;;显示日期
(setq display-time-day-and-date t)
;;显示时间
(display-time)
;;时间为24小时制
(setq display-time-24hr-format t)
;;时间显示包括日期和具体时间
(setq display-time-day-and-date t)
;;时间栏旁边启动邮件设置
(setq display-time-use-mail-icon t)
;;时间的变化频率
(setq display-time-interval 10)
 
;;光标靠近鼠标指针时，让鼠标指针自动让开，别挡住视线。
(mouse-avoidance-mode 'animate)
 
;;指针不闪，不恍花眼睛。
(blink-cursor-mode -1)
(transient-mark-mode 1)

;; 显示行号切换
(global-set-key [C-f6] 'global-linum-mode)
 
;;语法加亮
(global-font-lock-mode t)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 模块初始化 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; linum-mode 
(require 'linum)
(global-linum-mode t) 

;; org-mode 初始化
;; (require 'org-install)
(add-to-list 'auto-mode-alist '("\\.org$" . org-mode)) 
(define-key global-map "\C-cl" 'org-store-link)
(define-key global-map "\C-ca" 'org-agenda)
(setq org-log-done t)


(require 'onlyu-init)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 全局设定 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;y/n替代yes/no
(fset 'yes-or-no-p 'y-or-n-p)
 
;;设置粘贴缓冲条目数量
(setq kill-ring-max 200)
 
;;递归使用minibuffer
(setq enable-recursive-minibuffers t)
 
;;支持外部程序粘贴
(setq x-select-enable-clipboard t)
 
;; 默认 80 列自动换行, 需要 M-x auto-fill-mode 模式下
(setq default-fill-column 80)
 
;;取消错误铃，闪屏
(setq visible-bell t)
 
;;设置默认工作目录
(setq default-directory "~/")
 
;;默认为text模式
;;(setq default-major-mode 'org-mode)
 
;; 我的信息
(setq user-full-name "onlyu")
(setq user-mail-address "efiish@gmail.com")
 
; key setting
(setq mac-option-key-is-meta nil)
(setq mac-command-key-is-meta t)
(setq mac-command-modifier 'meta)
(setq mac-option-modifier 'ctl)
(setq mac-control-modifier nil)

;; 不使用alt键执行命令
(global-set-key "\C-x\C-m" 'execute-extended-command)
(global-set-key "\C-c\C-m" 'execute-extended-command)

(global-set-key "\C-w" 'backward-kill-word)
(global-set-key "\C-x\C-k" 'kill-region)
(global-set-key "\C-c\C-k" 'kill-region)

;(global-set-key (kbd "C-,") 'beginning-of-buffer)
;(global-set-key (kbd "C-.") 'end-of-buffer) 

; for macro
(global-set-key [f5] 'call-last-kbd-macro)

; for search
(global-set-key "\M-s" 'isearch-forward-regexp)
(global-set-key "\M-r" 'isearch-backward-regexp)

;; command alias
(defalias 'gf 'grep-find)
(defalias 'fd 'find-dired)
(defalias 'sh 'shell)

(defalias 'qrr 'query-replace-regexp)
(defalias 'lml 'list-matching-lines)
(defalias 'dml 'delete-matching-lines)
(defalias 'rof 'recentf-open-files)

(defalias 'eb 'eval-buffer)
(defalias 'er 'eval-region)
