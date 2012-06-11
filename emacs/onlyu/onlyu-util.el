(defun msdos-to-unix-file()
  "make line ends with \n"
  (interactive)
  (goto-char (point-min))
  (while (search-forward "\r\n" nil t)
    (replace-match "\n")))

(global-set-key "\C-i" 'ska-point-to-register)
(global-set-key "\C-o" 'ska-jump-to-register)
(defun ska-point-to-register()
  "Store cursorposition _fast_ in a register. 
Use ska-jump-to-register to jump back to the stored 
position."
  (interactive)
  (setq zmacs-region-stays t)
  (point-to-register 8))

(defun ska-jump-to-register()
  "Switches between current cursorposition and position
that was stored with ska-point-to-register."
  (interactive)
  (setq zmacs-region-stays t)
  (let ((tmp (point-marker)))
        (jump-to-register 8)
        (set-register 8 tmp)))

(require 'session)
(add-hook 'after-init-hook 'session-initialize)

;;(load "desktop")
(require 'desktop)
(desktop-load-default)
(add-hook 'after-init-hook 'desktop-read)
(add-hook 'kill-emacs-hook (lambda () (desktop-save "~/.emacs.d/")))

(provide 'onlyu-util)