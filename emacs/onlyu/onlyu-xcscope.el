
(require 'xcscope)
(define-key global-map "\C-cs" 'cscope-find-this-symbol)
(define-key global-map "\C-cd" 'cscope-find-global-definition)
(define-key global-map "\C-cf" 'cscope-find-this-file)
(define-key global-map "\C-cc" 'cscope-find-functions-calling-this-function)
(define-key global-map "\C-ct" 'cscope-find-this-text-string)
(define-key global-map "\C-ci" 'cscope-find-files-including-file)

(provide 'onlyu-xcscope)