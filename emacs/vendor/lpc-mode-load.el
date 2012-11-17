;;; lpc-mode-load.el --- Major mode for the Go programming language

;;; Commentary:

;; To install lpc-mode, add the following lines to your .emacs file:
;;   (add-to-list 'load-path "PATH CONTAINING lpc-mode-load.el" t)
;;   (require 'lpc-mode-load)
;; After this, lpc-mode will be used for files ending in '.go'.

;; To compile lpc-mode from the command line, run the following
;;   emacs -batch -f batch-byte-compile lpc-mode.el

;; See lpc-mode.el for documentation.

;;; Code:

;; To update this file, evaluate the following form
;;   (let ((generated-autoload-file buffer-file-name)) (update-file-autoloads "lpc-mode.el"))


;;;### (autoloads (lpc-mode) "lpc-mode" "lpc-mode.el" (19168 32439))
;;; Generated autoloads from lpc-mode.el

(autoload (quote lpc-mode) "lpc-mode" "\
Major mode for editing Go source text.

This provides basic syntax highlighting for keywords, built-ins,
functions, and some types.  It also provides indentation that is
\(almost) identical to gofmt.

\(fn)" t nil)

(add-to-list (quote auto-mode-alist) (cons "\\.go$" (function lpc-mode)))

;;;***

(provide 'lpc-mode-load)
