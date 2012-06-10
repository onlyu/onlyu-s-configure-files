(defun my-c-mode-common-hook ()
  (setq tab-width 4)
  (setq indent-tabs-mode t)
  (setq c-basic-offset 4)
  (hs-minor-mode) ;; ДњТыелЕў
  )

(defun onlyu-pike-mode-hook ()
  (auto-complete-mode))

(let ((filter (format "%s.*\\.c" fs-logic-dir)))
  (add-to-list 'auto-mode-alist (cons filter '(. pike-mode))))

(add-hook 'c-mode-common-hook 'my-c-mode-common-hook)
(add-hook 'pike-mode-hook 'onlyu-pike-mode-hook)

(defun my-make-CR-do-indent ()
  (define-key c-mode-base-map "\C-m" 'c-context-line-break))
(add-hook 'c-initialization-hook 'my-make-CR-do-indent)

(provide 'onlyu-cc-mode)