
;; �ҵ���Ϣ
(setq user-full-name "onlyu")
(setq user-mail-address "efiish@gmail.com")

(set-cursor-color "yellow")
(set-mouse-color "white")

;; color theme
(require 'color-theme)
(color-theme-initialize)
(color-theme-wombat)

;;����ճ��������Ŀ����
(setq kill-ring-max 200)
 
;;�ݹ�ʹ��minibuffer
(setq enable-recursive-minibuffers t)
 
;;֧���ⲿ����ճ��
(setq x-select-enable-clipboard t)
 
;; Ĭ�� 80 ���Զ�����, ��Ҫ M-x auto-fill-mode ģʽ��
(setq default-fill-column 80)
 
;;ȡ�������壬����
(setq visible-bell t)
 
;;����Ĭ�Ϲ���Ŀ¼
(setq default-directory "~/")
 
;;Ĭ��Ϊtextģʽ
;;(setq default-major-mode 'org-mode)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; �����ʾ ;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;������������
(setq inhibit-startup-message t)
;; *scratch* buffer����ʾ��Ϣ
(setq initial-scratch-message "")

;;�ߴ�
(setq initial-frame-alist '( (width . 80) (height . 25))) 
;;�����ʽ, "�ļ���  @  ȫ·���ļ���"
(setq frame-title-format '("%b@" buffer-file-name))
;;ȡ����ʾ������
(if (functionp 'tool-bar-mode) (setq tool-bar-mode nil)) 

;; ȡ����ʾ�˵���
(if (functionp 'menu-bar-mode) (setq menu-bar-mode nil))
 
;;ȥ��������, �����ִ���
(if (functionp 'set-scroll-bar-mode) (set-scroll-bar-mode nil))
 
;;������ʾ�к�
(setq column-number-mode t)

;;��ʾ����ƥ��
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
    (w32-maximize-frame)
    (custom-set-faces
     ;; custom-set-faces was added by Custom.
     ;; If you edit it by hand, you could mess it up, so be careful.
     ;; Your init file should contain only one such instance.
     ;; If there is more than one, they won't work right.
     '(default ((t (:inherit nil :stipple nil :background "black" :foreground "green" :inverse-video nil :box nil :strike-through nil :overline nil :underline nil :slant normal :weight normal :height 160 :width normal :foundry "outline" :family "Droid Sans Mono"))))
     '(cursor ((t (:background "yellow"))))
     '(linum ((t (:background "black" :foreground "red"))))
     '(linum-board ((t (:inherit (shadow default) :background "#888888"))))
     '(org-mode-default ((t (:inherit outline-mode-default :stipple nil :strike-through nil :underline nil :slant normal :weight normal :height 140 :width normal :family "Droid Sans Mono"))))))

(setq locale-coding-system 'utf-8)
(set-terminal-coding-system 'utf-8)
(set-keyboard-coding-system 'utf-8)
(set-selection-coding-system 'utf-8)
(prefer-coding-system 'utf-8)

(provide 'onlyu-custom)
