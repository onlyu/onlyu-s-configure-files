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
 '(ecb-layout-name "right1")
 '(ecb-options-version "2.40")
 '(fs-dir "F:/fs/trunk/" t)
 '(inverse-video t)
 '(ispell-dictionary "en")
 '(linum-delay t)
 '(linum-eager nil)
 '(linum-format "%4d")
 '(pop-up-frames nil)
 '(pop-up-windows t)
 '(python-indent 4)
 '(standard-indent 4))

(custom-set-faces
  ;; custom-set-faces was added by Custom.
  ;; If you edit it by hand, you could mess it up, so be careful.
  ;; Your init file should contain only one such instance.
  ;; If there is more than one, they won't work right.
 '(default ((t (:inherit nil :stipple nil :background "black" :foreground "green" :inverse-video nil :box nil :strike-through nil :overline nil :underline nil :slant normal :weight normal :height 180 :width normal :foundry "apple" :family "Menlo"))))
 '(linum ((t (:background "black" :foreground "red"))))
 '(linum-board ((t (:inherit (shadow default) :background "#888888"))))
 '(org-mode-default ((t (:inherit outline-mode-default :stipple nil :strike-through nil :underline nil :slant normal :weight normal :height 180 :width normal :family "Monaco")))))

(require 'init)
