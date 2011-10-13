(defun my-c-mode-common-hook ()
  (setq tab-width 4)
  (setq indent-tabs-mode t)
  (setq c-basic-offset 4))
  
(add-hook 'c-mode-common-hook 'my-c-mode-common-hook)

(defun my-make-CR-do-indent ()
  (define-key c-mode-base-map "\C-m" 'c-context-line-break))
(add-hook 'c-initialization-hook 'my-make-CR-do-indent)

(provide 'onlyu-cc-mode)