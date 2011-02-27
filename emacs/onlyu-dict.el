;; look-up english word

(require 'get-url-content)

(defgroup onlyu-dict nil
  "look-up word with google, thank you google!!"
    :group 'convenience)
(defcustom dict-url "www.google.com/dictionary/json?callback=dict_api.callbacks.id100&q=%s&sl=en&tl=zh-CN&restrict=pr,de&client=te"
  ""
  :group 'linum
  :type 'sexp)

(defun onlyu-find-word (word)
  "find the word in google dict, return it's means"
  (interactive "P\nsworld: ")
  (get-url-content (format dict-url word)))
(provide 'onlyu-dict)
