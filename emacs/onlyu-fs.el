(defconst fs-version "0.1")

(defgroup fs nil
  "emacs mode for fengshen project"
  :group 'convenience)

(defcustom fs-dir "F:/fs/trunk/"
  "the root directory of fengshen project codes"
  :group 'fs
  :type 'sexp)

(defun fs-script-dir ()
  "fengshen engine's script directory"
  (concat fs-dir "script"))

(defun fs-engine-dir()
  "fengshen engine's engine code directory"
  (concat fs-dir "fsengine"))

(defun fs-goto-script()
  "goto the fengshen's script directory"
  (interactive)
  (find-file (fs-script-dir)))

(defun run-file-at-dir (dir file)
  (progn (setq old-default-directory default-directory) ; 
	 (setq default-directory dir)		; 
	 (start-file-process "fsgame-client" "fsgame-client" (concat fs-dir file))
	 (setq default-directory old-default-directory))) ; 
(defun fs-run(file)
  "execute a file in fs-dir directory"
  (interactive)
  (run-file-at-dir fs-dir file))

(defun fs-start()
  "start fengshen's game"
  (interactive)
  (fs-run "start.bat"))

(defun fs-build()
  "build fengshen's engine"
  (interactive)
  (fs-run "build-release.bat"))

(defun fs-lua-editor()
  "start fengshen's ui editor and scene editor"
  (interactive)
  (fs-run "luaeditor.bat"))

(defun fs-python-editor()
  "start fengshen's resource editor"
  (interactive)
  (fs-run "pyeditor.bat"))

(defun fs-update-editors()
  "rebuild the editors and submit to tool's svn"
  (interactive)
  (run-file-at-dir (concat fs-dir "../build/autobuild/_fs_tool_update_editors.bat")))

(global-set-key [f9] 'fs-start)
(global-set-key [f10] 'fs-lua-editor)
(global-set-key [f11] 'fs-python-editor)

(provide 'onlyu-fs)