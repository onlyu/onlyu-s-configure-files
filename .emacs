;; load path
(add-to-list 'load-path "~/emacs/")

(custom-set-variables
 ;; custom-set-variables was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 '(ac-compatible-packages-regexp "^ac-")
 '(ac-delay 0.0)
 '(ac-expand-on-auto-complete nil)
 '(blink-cursor-mode nil)
 '(column-number-mode t)
 '(dired-garbage-files-regexp "\\(?:\\.\\(?:aux\\|bak\\|dvi\\|log\\|orig\\|rej\\|toc\\|lo\\)\\)\\'")
 '(dired-omit-files "^\\.?#\\|^\\.$\\|^\\.\\.$\\|^.*\\.lo$\\|^.*~$\\|^.svn$")
 '(display-time-mode t)
 '(ecb-layout-name "right1")
 '(ecb-options-version "2.40")
 '(fs-dir "F:/fs/trunk/")
 '(inverse-video t)
 '(ispell-dictionary "en")
 '(linum-delay t)
 '(linum-eager nil)
 '(linum-format "%4d")
 '(menu-bar-mode nil)
 '(message-send-mail-function (quote message-send-mail-with-sendmail))
 '(pop-up-frames nil)
 '(pop-up-windows t)
 '(python-indent 4)
 '(send-mail-function (quote sendmail-send-it))
 '(show-paren-mode t)
 '(standard-indent 4)
 '(tool-bar-mode nil))

(custom-set-faces
 ;; custom-set-faces was added by Custom.
 ;; If you edit it by hand, you could mess it up, so be careful.
 ;; Your init file should contain only one such instance.
 ;; If there is more than one, they won't work right.
 '(default ((t (:inherit nil :stipple nil :background "black" :foreground "green" :inverse-video nil :box nil :strike-through nil :overline nil :underline nil :slant normal :weight normal :height 143 :width normal :foundry "outline" :family "Courier New"))))
 '(cursor ((t (:background "yellow"))))
 '(linum-board ((t (:inherit (shadow default) :background "#888888"))))
 '(linum ((t (:background "black" :foreground "red"))))
 '(org-mode-default ((t (:inherit outline-mode-default :stipple nil :strike-through nil :underline nil :slant normal :weight normal :height 140 :width normal :family "Menlo")))))

(require 'init)
