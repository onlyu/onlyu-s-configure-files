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
 '(ecb-layout-name "right1")
 '(ecb-options-version "2.40")
 '(fs-dir "F:/fs/trunk/" t)
 '(inverse-video t)
 '(ispell-dictionary "en")
 '(linum-delay t)
 '(linum-eager nil)
 '(linum-format "%4d")
 '(python-indent 8)
 '(standard-indent 8))

(custom-set-faces
  ;; custom-set-faces was added by Custom.
  ;; If you edit it by hand, you could mess it up, so be careful.
  ;; Your init file should contain only one such instance.
  ;; If there is more than one, they won't work right.
 '(default ((t (:inherit nil :stipple nil :background "black" :foreground "green" :inverse-video nil :box nil :strike-through nil :overline nil :underline nil :slant normal :weight normal :height 150 :width normal :foundry "outline" :family "鏂板畫浣�"))))
 '(linum ((t (:background "black" :foreground "red"))))
 '(linum-board ((t (:inherit (shadow default) :background "#888888")))))



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 妯″潡鍒濆鍖� ;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; linum-mode 
(require 'linum)
(global-linum-mode t) 

;; org-mode 鍒濆鍖�
;; (require 'org-install)
(add-to-list 'auto-mode-alist '("\\.org$" . org-mode)) 
(define-key global-map "\C-cl" 'org-store-link)
(define-key global-map "\C-ca" 'org-agenda)
(setq org-log-done t)


(require 'onlyu-init)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 鍏ㄥ眬璁惧畾 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;y/n鏇夸唬yes/no
(fset 'yes-or-no-p 'y-or-n-p)

; for macro
(global-set-key [f5] 'call-last-kbd-macro)

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
