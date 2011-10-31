;; off means onlyu-find-func

;; this module base on find-func.el
(defun off-find-at-point ()
  "Find the function or variable or library at point"
  (interactive)
  (let* ((var (variable-at-point))
	 (fun (function-called-at-point)))
    (if (and (symbolp var)
	     (not (equal var fun)))
	(find-variable var)
      (if (and fun (symbolp fun) (equal var fun)) 
	  (find-function fun)
	(find-library (thing-at-point 'symbol))))
    ))

(defun off-describe-at-point ()
  (interactive)
  (let* ((var (variable-at-point))
	 (fun (function-called-at-point)))
    (if (and (symbolp var)
	     (not (equal var fun)))
	(describe-variable var)
      (describe-function fun))
    ))

(defun off-init-key-map (map)
  (define-key map "\C-cg" 'off-find-at-point)
  (define-key map "\C-cl" 'off-describe-at-point))

;; the key map like my xcscope keymap:
;; 1. \C-cg goto the function defination without promt
;; 2. \C-cl show the help message of the function without prompt

(defun off-lisp-mode-hook ()
  (off-init-key-map emacs-lisp-mode-map))

(add-hook 'emacs-lisp-mode-hook 'off-lisp-mode-hook)

(defun off-lisp-interaction-mode-hook()
  (off-init-key-map lisp-interaction-mode-map))

(add-hook 'lisp-interaction-mode-hook 'off-lisp-interaction-mode-hook)

(provide 'onlyu-find-func)