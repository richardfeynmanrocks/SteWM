#include <X11/Xlib.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

int main(void)
{
    Display * dpy;
    XWindowAttributes attr;
    XButtonEvent start;
    XEvent ev;

    if(!(dpy = XOpenDisplay(0x0))) return 1;

    XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("F1")), Mod1Mask,
            DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync);
    XGrabButton(dpy, 1, Mod1Mask, DefaultRootWindow(dpy), True,
            ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);
    XGrabButton(dpy, 3, Mod1Mask, DefaultRootWindow(dpy), True,
            ButtonPressMask|ButtonReleaseMask|PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None);

    /* XSelectInput(dpy, DefaultRootWindow(dpy), SubstructureRedirectMask | SubstructureNotifyMask | ResizeRedirectMask); */

    start.subwindow = None;

    for(;;)
    {
      	XNextEvent(dpy, &ev);

	Window root, parent;
	Window *children;
	unsigned int nchildren;

	int snum = DefaultScreen(dpy);
	XQueryTree(dpy, DefaultRootWindow(dpy), &root, &parent, &children, &nchildren);
	int num_viewable = 0;
	for (int i = 0; i < nchildren; i++) {
	    XGetWindowAttributes(dpy, children[i], &attr);
	    if (attr.map_state == IsViewable) num_viewable += 1;        	    
	}
	for (int i = 0; i < nchildren; i++) {
	    XGetWindowAttributes(dpy, children[i], &attr);
	    if (attr.map_state == IsViewable) {
		XMoveResizeWindow(dpy, children[i], i*DisplayWidth(dpy, snum)/num_viewable, 0, DisplayWidth(dpy, snum)/num_viewable, DisplayHeight(dpy, snum));
	    }
	}

	/* if(ev.type == CreateNotify) { */
	/*     int snum = DefaultScreen(dpy); */
	/*     XGetWindowAttributes(dpy, ev.xcreatewindow.window, &attr); */
        /*     XMoveResizeWindow(dpy, ev.xcreatewindow.window, 500, 500, 100, 100); */
	/* } */
        if(ev.type == KeyPress && ev.xkey.subwindow != None)
            XRaiseWindow(dpy, ev.xkey.subwindow);
        else if(ev.type == ButtonPress && ev.xbutton.subwindow != None)
        {
            XGetWindowAttributes(dpy, ev.xbutton.subwindow, &attr);
            start = ev.xbutton;
        }
        else if(ev.type == MotionNotify && start.subwindow != None)
        {
            int xdiff = ev.xbutton.x_root - start.x_root;
            int ydiff = ev.xbutton.y_root - start.y_root;
            XMoveResizeWindow(dpy, start.subwindow,
                attr.x + (start.button==1 ? xdiff : 0),
                attr.y + (start.button==1 ? ydiff : 0),
                MAX(1, attr.width + (start.button==3 ? xdiff : 0)),
                MAX(1, attr.height + (start.button==3 ? ydiff : 0)));
        }
        else if(ev.type == ButtonRelease) {
            start.subwindow = None;
	}
    }
}

