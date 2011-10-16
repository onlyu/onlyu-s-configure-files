
;; windows key setting
(setq w32-pass-lwindow-to-system nil 
      w32-pass-rwindow-to-system nil 
      w32-pass-apps-to-system nil 
      w32-lwindow-modifier 'super ;; Left Windows key 
      w32-rwindow-modifier 'super ;; Right Windows key 
      w32-apps-modifier 'hyper) ;; Menu key

;; mac key setting
(setq mac-command-modifier 'meta)
(setq mac-option-modifier 'nil)
(setq mac-control-modifier 'control)


;; key binddings
(defun kill-line-or-region()
  (interactive)
  (let (pos1 pos2)
    (if (and transient-mark-mode mark-active)
	(progn
	  (setq pos1 (region-beginning) pos2 (region-end))
	  (kill-region pos1 pos2))
      (kill-line))
  ))
(global-set-key "\C-k" 'kill-line-or-region)
(global-set-key "\C-w" 'backward-kill-word)
(global-set-key "\M-Z" 'redo)
(global-set-key "\M-z" 'undo)

;; (global-set-key "\M-p" 'beginning-of-buffer)
;; (global-set-key "\M-n" 'end-of-buffer)

;; (global-set-key "\M- " 'set-mark-command)
;; (global-set-key [deletechar] 'delete-backward-char)
;; (global-set-key "\C-h" 'delete-backward-char)

(provide 'onlyu-key)