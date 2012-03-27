package
{
    import flash.display.*;
    public final class ResourceManager
    {
	[Embed(source="./spritelib_gpl/platform/dino.png")]
	public static var BrownPlane:Class;
	public static var BrownPlaneGraphics:GraphicsResource = new GraphicsResource(new BrownPlane());
    }
}