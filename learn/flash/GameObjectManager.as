package
{
    import mx.core.*;
    import mx.collections.*;
    import flash.display.*;
    
    public class GameObjectManager
    {
	// double buffer
	public var backBuffer:BitmapData;
	public var clearColor:uint = 0xFF0043AB;
	protected static var instance:GameObjectManager = null;
	protected var lastFrame:Date;

	// object management
	protected var gameObjects:ArrayCollection = new ArrayCollection();
	protected var newGameObjects:ArrayCollection = new ArrayCollection();
	protected var removedGameObjects:ArrayCollection = new ArrayCollection();
	
	static public function getInstance():GameObjectManager{
	    if (instance == null)
		instance = new GameObjectManager();
	    return instance;
	}

	public function GameObjectManager() {
	    if (instance != null)
		throw new Error("only one singleton instance should be instantiated");
	    backBuffer = new BitmapData(FlexGlobals.topLevelApplication.width,
					FlexGlobals.topLevelApplication.height,
					false);
	}

	public function startup():void {
	    lastFrame = new Date();
	    new Bounce().startupBounce();
	}

	public function shutdown():void {
	    shutdownAll();
	}

	public function enterFrame():void {
	    var thisFrame:Date = new Date();
	    var seconds:Number=(thisFrame.getTime() - lastFrame.getTime())/1000.0;
	    lastFrame = thisFrame;

	    for each(var gameObject:GameObject in gameObjects) {
		if (gameObject.inuse)
		    gameObject.enterFrame(seconds);
	    }
	    drawObjects();
	}

	protected function drawObjects():void {
	    backBuffer.fillRect(backBuffer.rect, clearColor);

	    for each(var gameObject:GameObject in gameObjects) {
		if (gameObject.inuse)
		    gameObject.copyToBackBuffer(backBuffer);
	    }
	}

	public function addGameObject(gameObject:GameObject): void {
	    newGameObjects.addItem(gameObject);
	}

	public function removeGameObject(gameObject:GameObject): void {
	    removedGameObjects.addItem(gameObject);
	}

	protected function shutdownAll(): void {
	    for each(var gameObject:GameObject in gameObject) {
		var found:Boolean = false;
		for each (var removedObject:GameObject in removedGameObjects) {
		    if (removedObject == gameObject) {
			found = true;
			break;
		    }
		}

		if (!found)
		    gameObject.shutdown();
	    }
	}

	protected function insertNewGameObjects(): void {
	    for each (var gameObject:GameObject in newGameObjects) {
		for (var i:int = 0; i<gameObjects.length; ++i) {
		    if (gameObjects.getItemAt(i).zOrder > gameObject.zOrder || gameObjects.getItemAt(i).zOrder == -1)
			break;
		}
		gameObjects.addItemAt(gameObject, i);
	    }
	    newGameObjects.removeAll();
	}

	protected function removeDeletedGameObjects(): void {
	    for each (var gameObject:GameObject in removedGameObjects) {
		var i:int = 0;
		for (i = 0; i<gameObjects.length; ++i) {
		    if (gameObjects.getItemAt(i) == gameObject) {
			gameObjects.removeItemAt(i);
			break;
		    }
		}
	    }
	    removedGameObjects.removeAll();
	}
    }
}