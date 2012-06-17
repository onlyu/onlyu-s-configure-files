;; load path
(add-to-list 'load-path "~/emacs/")
(require 'init)
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
 '(completion-ignored-extensions (quote (".o" "~" ".bin" ".bak" ".obj" ".map" ".ico" ".pif" ".lnk" ".a" ".ln" ".blg" ".bbl" ".dll" ".drv" ".vxd" ".386" ".elc" ".lof" ".glo" ".idx" ".lot" ".svn/" ".hg/" ".git/" ".bzr/" "CVS/" "_darcs/" "_MTN/" ".fmt" ".tfm" ".class" ".fas" ".lib" ".mem" ".x86f" ".sparcf" ".fasl" ".ufsl" ".fsl" ".dxl" ".pfsl" ".dfsl" ".p64fsl" ".d64fsl" ".dx64fsl" ".lo" ".la" ".gmo" ".mo" ".toc" ".aux" ".cp" ".fn" ".ky" ".pg" ".tp" ".vr" ".cps" ".fns" ".kys" ".pgs" ".tps" ".vrs" ".pyc" ".pyo")))
 '(dired-omit-files "^\\.?#\\|^\\.$\\|^\\.\\.$\\|^.*\\.lo$\\|^.*~$")
 '(display-time-mode t)
 '(ecb-layout-name "right1")
 '(ecb-options-version "2.40")
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
 '(linum ((t (:background "black" :foreground "red"))))
 '(linum-board ((t (:inherit (shadow default) :background "#888888"))))
 '(org-mode-default ((t (:inherit outline-mode-default :stipple nil :strike-through nil :underline nil :slant normal :weight normal :height 140 :width normal :family "Menlo")))))


