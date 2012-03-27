package
{
    import flash.geom.*;
    import mx.core.*;
    
    public class Bounce extends GameObject {
	protected static const speed:Number = 100;
	protected var direction:Point = new Point(1,1);
	
	public function Bounce() {
	    super();
	}

	public function startupBounce():void {
	    super.startupGameObject(ResourceManager.BrownPlaneGraphics, new Point(0,0));
	}
	
	override public function shutdown():void {
	    super.shutdown();
	}

	override public function enterFrame(dt:Number):void {
	    super.enterFrame(dt);
	    position.x += direction.x * speed * dt;
	    position.y += direction.y * speed * dt;
	    if (position.x > FlexGlobals.topLevelApplication.width - graphics.bitmap.width ) 
		direction.x = -1;
	    else if(position.x < 0)
		direction.x = 1;

	    if (position.y > FlexGlobals.topLevelApplication.height - graphics.bitmap.height)
		direction.y = -1;
	    else if (position.y < 0)
		direction.y = 1;
	}
    }
}