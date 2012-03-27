package
{
    import flash.display.*;
    import flash.events.*;
    import flash.geom.*;
    
    public class GameObject {
	public var position:Point = new Point(0,0);
	public var zOrder:int = 0;
	public var graphics:GraphicsResource = null;
	public var inuse:Boolean = false;
	
	public function GameObject() {

	}
	
	public function startupGameObject(graphics:GraphicsResource, position:Point, z:int = 0): void {
	    if (!inuse) {
		this.graphics = graphics;
		this.zOrder = z;
		this.position = position.clone();
		this.inuse = true;
		GameObjectManager.getInstance().addGameObject(this);
	    }
	}
	
	public function shutdown():void {
	    if (inuse) {
		graphics = null;
		inuse = false;
		GameObjectManager.getInstance().removeGameObject(this);
	    }
	}

	public function copyToBackBuffer(db:BitmapData):void{
	    db.copyPixels(graphics.bitmap, graphics.bitmap.rect, position, graphics.bitmapAlpha, new Point(0,0), true);
	}

	public function enterFrame(dt:Number):void {
	    
	}
    }
}