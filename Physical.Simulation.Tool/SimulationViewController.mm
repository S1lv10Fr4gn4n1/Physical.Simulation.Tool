//
//  ViewController.m
//  Physical.Simulation.Tool
//
//  Created by Silvio Fragnani da Silva on 11/06/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "SimulationViewController.h"

@interface SimulationViewController ()
- (void)setupGL;
- (void)tearDownGL;
- (void)initializeGestureRecognizer:(UIView *)view;
@end

static EAGLContext * context;

@implementation SimulationViewController

- (void)viewDidLoad
{
    [super viewDidLoad];

    if (!context) {
        context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
        
        if (!context) {
            NSLog(@"Failed to create ES context");
        }
    }
    
    GLKView *view = (GLKView *)self.view;
    view.context = context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;

    [self initializeGestureRecognizer: view];
    
    [self setupGL];
    
    
    [NSTimer scheduledTimerWithTimeInterval:3 
                                     target:self 
                                   selector:@selector(timeCmd:) 
                                   userInfo:NULL 
                                    repeats:YES];

}

- (void)timeCmd:(NSTimer*)theTimer
{
    NSLog(@"oi");
}

- (void)viewDidUnload
{    
    [super viewDidUnload];
    
    [self tearDownGL];
    
    if ([EAGLContext currentContext] == context) {
        [EAGLContext setCurrentContext:nil];
    }
    
	context = nil;
}

- (void)loadView
{
    [super loadView];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Release any cached data, images, etc. that aren't in use.
    
    UIAlertView * alert = [[UIAlertView alloc]initWithTitle:@"Memory Warning" 
                                                    message:@"Memory Warning" 
                                                   delegate:self 
                                          cancelButtonTitle:@"Ok" 
                                          otherButtonTitles: nil];
    [alert show];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    if (!self.view) {
        Controller::getInstance()->resizeScreen(self.view.bounds.size.width, self.view.bounds.size.height);
    }
    
    return UIInterfaceOrientationIsLandscape(interfaceOrientation);
}

- (void)setupGL
{
    [EAGLContext setCurrentContext:context];
    
    if (!Controller::getInstance()->isInitialized()) {
        Controller::getInstance()->initializeEngine();
        Controller::getInstance()->initializeContextOpenGLES();
    }
}

- (void)tearDownGL
{
    [EAGLContext setCurrentContext:context];
    
    Controller::getInstance()->freeObjects();
}

- (void)initializeGestureRecognizer:(UIView *)view
{
    UITapGestureRecognizer *doubleTapTwoFinger = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(doubleTapTwoFingerDetected:)];
    doubleTapTwoFinger.numberOfTapsRequired = 2;
    doubleTapTwoFinger.numberOfTouchesRequired = 2;
    [view addGestureRecognizer:doubleTapTwoFinger];

    UITapGestureRecognizer *doubleTapOneFinger = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(doubleTapOneFingerDetected:)];
    doubleTapOneFinger.numberOfTapsRequired = 2;
    doubleTapOneFinger.numberOfTouchesRequired = 1;
    [view addGestureRecognizer:doubleTapOneFinger];

    UIPinchGestureRecognizer *pinchRecognizer = [[UIPinchGestureRecognizer alloc]initWithTarget:self action:@selector(pinchDetected:)];
    [view addGestureRecognizer:pinchRecognizer];
    
    UIRotationGestureRecognizer *rotationRecognizer = [[UIRotationGestureRecognizer alloc]initWithTarget:self action:@selector(rotationDetected:)];
    [view addGestureRecognizer:rotationRecognizer];
    
    UISwipeGestureRecognizer *swipeRightRecognizer = [[UISwipeGestureRecognizer alloc]initWithTarget:self action:@selector(swipeRightDetected:)];
    swipeRightRecognizer.direction = UISwipeGestureRecognizerDirectionRight;
    [view addGestureRecognizer:swipeRightRecognizer];

    UISwipeGestureRecognizer *swipeLeftRecognizer = [[UISwipeGestureRecognizer alloc]initWithTarget:self action:@selector(swipeLeftDetected:)];
    swipeLeftRecognizer.direction = UISwipeGestureRecognizerDirectionLeft;
    [view addGestureRecognizer:swipeLeftRecognizer];
    
    UILongPressGestureRecognizer *longPressRecognizer = [[UILongPressGestureRecognizer alloc]initWithTarget:self action:@selector(longPressDetected:)];
    longPressRecognizer.minimumPressDuration = 2;
    longPressRecognizer.numberOfTouchesRequired = 1;
    [view addGestureRecognizer:longPressRecognizer];
}

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event
{
//    NSLog(@"touchesBegan");
}

- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event
{
//    NSLog(@"touchesCancelled");
}

- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event
{
//    NSLog(@"touchesEnded");
}

- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event
{
//    NSLog(@"touchesMoved");
}

- (IBAction)longPressDetected:(UIGestureRecognizer *)sender 
{
//    NSLog(@"Long Press");
}

- (IBAction)swipeRightDetected:(UIGestureRecognizer *)sender
{
//    NSLog(@"Right Swipe");
}

- (IBAction)swipeLeftDetected:(UIGestureRecognizer *)sender
{
//    NSLog(@"Left Swipe");
}

- (IBAction)doubleTapTwoFingerDetected:(UIGestureRecognizer *)sender
{
    Controller::getInstance()->stopSimulation();
    
    [self dismissViewControllerAnimated:YES completion:nil];

    [[UIApplication sharedApplication] setStatusBarHidden:NO withAnimation:UIStatusBarAnimationNone];
}

- (IBAction)doubleTapOneFingerDetected:(UIGestureRecognizer *)sender
{
    CGPoint cgPoint = [sender locationInView:sender.view];
    
    Controller::getInstance()->selectedSimulatedObject(MakePointer(cgPoint.x, cgPoint.y, 0));
}

- (IBAction)pinchDetected:(UIGestureRecognizer *)sender
{    
//    CGFloat scale = [(UIPinchGestureRecognizer *)sender scale];
//    CGFloat velocity = [(UIPinchGestureRecognizer *)sender velocity];
//    
//    NSLog(@"%@",[[NSString alloc] initWithFormat: @"Pinch - scale = %f, velocity = %f", scale, velocity]);
}

- (IBAction)rotationDetected:(UIGestureRecognizer *)sender
{
//    CGFloat radians = [(UIRotationGestureRecognizer *)sender rotation];
//    CGFloat velocity = [(UIRotationGestureRecognizer *)sender velocity];
//    
//    NSLog(@"%@",[[NSString alloc] initWithFormat: @"Rotation - Radians = %f, velocity = %f",radians, velocity]);
}

#pragma mark - GLKView and GLKViewController delegate methods
- (void)update
{
    Controller::getInstance()->updateInformation();
}

- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect
{
    Controller::getInstance()->draw();
}

@end
