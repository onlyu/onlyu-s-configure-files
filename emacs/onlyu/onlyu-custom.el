
;; 我的信息
(setq user-full-name "onlyu")
(setq user-mail-address "efiish@gmail.com")

(set-cursor-color "yellow")
(set-mouse-color "white")

;; color theme
(require 'color-theme)
(color-theme-initialize)
(color-theme-wombat)

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
(if (functionp 'tool-bar-mode) (setq tool-bar-mode nil)) 

;; 取消显示菜单栏
(if (functionp 'menu-bar-mode) (setq menu-bar-mode nil))
 
;;去掉滚动条, 鼠标滚轮代替
(if (functionp 'set-scroll-bar-mode) (set-scroll-bar-mode nil))
 
;;底栏显示列号
(setq column-number-mode t)

;;显示括号匹配
(show-paren-mode t)
(setq show-paren-style 'parenthesis)

(setq display-time-day-and-date t)
(display-time)
(setq display-time-24hr-format t)
(setq display-time-day-and-date t)
(setq display-time-use-mail-icon t)
(setq display-time-interval 10)
 
;;(mouse-avoidance-mode 'animate)
 
(blink-cursor-mode -1)
(transient-mark-mode 1)

(global-font-lock-mode t)

(defun w32-maximize-frame ()
  "Maximize the current frame"
  (interactive)
  (w32-send-sys-command 61488))

(when (featurep 'Aquamacs)
  (aquamacs-toggle-full-frame))
(when (featurep 'emacsw32)
    (w32-maximize-frame))

(provide 'onlyu-custom)
