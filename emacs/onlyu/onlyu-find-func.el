;; off means onlyu-find-func

;; this module base on find-func.el
(defun off-function-at-point ()
  "Find the function at point"
  (interactive)
  (find-function (function-called-at-point)))

(defun off-variable-at-point ()
  "Find the variable at point"
  (interactive)
  (find-variable (variable-at-point)))

(defun off-library-at-point ()
  "Find the library at point"
  (interactive)
  (find-library (thing-at-point 'symbol)))

(defun off-describe-at-point ()
  (interactive)
  (describe-function (function-called-at-point)))

(defun off-init-key-map (map)
  (define-key map "\C-cg" 'off-function-at-point)
  (define-key map "\C-cl" 'off-describe-at-point))

(defun off-lisp-mode-hook ()
  (off-init-key-map emacs-lisp-mode-map))

(add-hook 'emacs-lisp-mode-hook 'off-lisp-mode-hook)

(defun off-lisp-interaction-mode-hook()
  (off-init-key-map lisp-interaction-mode-map))

(add-hook 'lisp-interaction-mode-hook 'off-lisp-interaction-mode-hook)

(provide 'onlyu-find-func)