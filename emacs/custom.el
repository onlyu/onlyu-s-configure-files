(custom-set-variables
  ;; custom-set-variables was added by Custom.
  ;; If you edit it by hand, you could mess it up, so be careful.
  ;; Your init file should contain only one such instance.
  ;; If there is more than one, they won't work right.
 '(column-number-mode t)
 '(cua-mode t nil (cua-base))
 '(inverse-video t)
 '(python-indent 8)
 '(show-paren-mode t)
 '(standard-indent 8))

(custom-set-faces
  ;; custom-set-faces was added by Custom.
  ;; If you edit it by hand, you could mess it up, so be careful.
  ;; Your init file should contain only one such instance.
  ;; If there is more than one, they won't work right.
 '(default ((t (:inherit nil :stipple nil :background "black" :foreground "white" :inverse-video nil :box nil :strike-through nil :overline nil :underline nil :slant normal :weight normal :height 151 :width normal :foundry "outline" :family "ËÎÌו")))))

;; key bindings

; setting the PC keyboard's various keys to Super or Hyper
; presumes you are using the emacsw32 distro on windows
; http://ourcomments.org/Emacs/EmacsW32Util.html
(setq w32-pass-lwindow-to-system nil 
      w32-pass-rwindow-to-system nil 
      w32-pass-apps-to-system nil 
      w32-lwindow-modifier 'super ;; Left Windows key 
      w32-rwindow-modifier 'super ;; Right Windows key 
      w32-apps-modifier 'hyper) ;; Menu key

;; make cursor movement keys under right hand's home-row.
;(global-set-key (kbd "s-s") 'backward-char) ; was indent-new-comment-line
;(global-set-key (kbd "s-f") 'forward-char)  ; was downcase-word
;(global-set-key (kbd "s-e") 'previous-line) ; was tab-to-tab-
;(global-set-key (kbd "s-d") 'next-line) ; was kill-sentence

(global-set-key (kbd "s-SPC") 'set-mark-command) ; was just-one-space
;; type parens in pairs with Hyper and right hands's home-row
(global-set-key (kbd "s-h") (lambda () (interactive) (insert "{}") (backward-char 1)))
(global-set-key (kbd "s-j") (lambda () (interactive) (insert "()") (backward-char 1)))
(global-set-key (kbd "s-k") (lambda () (interactive) (insert "[]") (backward-char 1)))


;; frequently use command
(global-set-key (kbd "H-d") 'goto-xy2)
(global-set-key (kbd "<home>") 'home)
(global-set-key (kbd "s-o") 'find-file)
(global-set-key (kbd "<f5>") 'indent-region)
(global-set-key (kbd "<f6>") 'comment-region)
