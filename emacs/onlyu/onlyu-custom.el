
;; �ҵ���Ϣ
(setq user-full-name "onlyu")
(setq user-mail-address "efiish@gmail.com")

;(set-cursor-color "ivory3")
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
(if (functionp 'tool-bar-mode) (tool-bar-mode nil)) 

;; ȡ����ʾ�˵���
(if (functionp 'menu-bar-mode) (menu-bar-mode nil))
 
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

(eval-when-compile
  (when (featurep 'Aquamacs)
    (aquamacs-toggle-full-frame)))

(provide 'onlyu-custom)