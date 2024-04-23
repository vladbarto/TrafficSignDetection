# Traffic Sign Detection

## Authors
`github.com/NataliaBoncea`, `github.com/vladbarto`

## Starting problem
![client_req](./Media/client_requirements.PNG)

	Being given colour images featuring circular, triangular or rectangular traffic signs, 
	a method of detection of all traffic signs from these images will be studied and implemented.

	As a result, detected traffic signs (or areas of those) will be marked over original images 
	of traffic scenes.

## Modules
- `Documentation_TSD_PI_SS2023/24.pdf`: Documentation (english)
	- it's WIP; to edit the document, [see this link](https://docs.google.com/document/d/14YRY6KIQ4P90PXBVY7GbaN0VgnwQGLwolHiTl9EFmdM/edit?usp=sharing)
- `Media`: folder with pictures for README only

## Workflow (pipeline)
![pipeline](./Media/pipeline.jpeg)

## Resources
- Color detection & Object Tracking (OpenCV Tutorials): https://www.opencv-srf.com/2010/09/object-detection-using-color-seperation.html
- Shape detection & Tracking using Contours (OpenCV Tutorials): https://www.opencv-srf.com/2011/09/object-detection-tracking-using-contours.html
- Cod rutier: Clasificare semne de circulatie: https://www.codrutier.ro/semne-de-circulatie
- Hough transformation implementation c++ (for detecting edges): https://www.keymolen.com/2013/05/hough-transformation-c-implementation.html
- Hough transform explanations (mathematically): https://www.youtube.com/watch?v=t1GXMvK9m84&t=263s
- Hough transforn explanations (computer vision - wise): https://www.youtube.com/watch?v=6yVMpaIoxIU
- Hough transform explained (german): https://www.youtube.com/watch?v=8fyWszvPCQE&t=23s
  
- ---
- Aging algorithm: https://www.youtube.com/watch?v=kJfa2HsTtlg
- Whole pipeline: https://durak-bolat.de/rsdc.html
- Improvements for TSR: https://pdf.sciencedirectassets.com/277811/1-s2.0-S1877042812X00247/1-s2.0-S1877042812043923/main.pdf?X-Amz-Security-Token=IQoJb3JpZ2luX2VjEN7%2F%2F%2F%2F%2F%2F%2F%2F%2F%2FwEaCXVzLWVhc3QtMSJGMEQCICZeUA9ee0I%2BOZPzRLNUz9OIfxOCUvfiJrsEvNuD%2FIKcAiAq2kxpwZr27mQKnriuan9HNfjk%2BNvD%2BJfInZaDFs5y8yq8BQj3%2F%2F%2F%2F%2F%2F%2F%2F%2F%2F8BEAUaDDA1OTAwMzU0Njg2NSIM%2B55JRUbTedSh2yqoKpAFTWCWYxLfaITFVtRwb6pzOq8gjDm2DnGipNJ0GfmnflG6WWe0cc7KyxbmNH6xhFc3LVPGtuYUKBp23Hs4ng6FhQ2nHzrKxjFA%2BR%2FTMuGSRuUvhbyvkAcsPOB0GYA4ij2JZPfMP2dB4WZwzO2yZYksvH1tiVuz2GEPFE7VzDk9b5hsv6CIq5kjc71kzqQ%2Bww%2FB0eCDU8Lo6pM0GrBOYyMRAtoAe3rfvkjaSpL3melQx5FYy5g4%2Faeo5L6wS%2Fh%2BrnD9BKcn6eurC7RsmhlONc9cLA8T%2BtVTdjeSkxGIKOUpzSY9Dt2v9%2B7OFLLgfgoY1gH8k75hhZ52OELID%2BJ1ObWU0EyXBQI7aYk1Ykvdo1aN9j74dKkMbAKrN1pPHEJ4Lnf9H7WAzxTKIyJAVnyvERQL4uWkMXhNx0BF2Ew4gfWABlU2oLDMEFEzWaGYQPiBVy8l3gwHlAklI9002mn3p2WDrh%2Fvj%2B9CeteTxl%2BhyF5O5L0R%2FFJaSe9%2FoRlsM7fLuhxYom8SFVzzNnees6NCxgf25W40Ezx6pKpeB4bMGLVf4YwUMeCX9zdXmhsY7vpzch8pAkBel1%2FewcAo8QFoqr7hP5qnz9tL7SfnkIH6D%2FsVsNnx86EVfEhnUaRPws4e1pb%2BozRBqgbabjAvVC7CPotOg7Es0QBN8KpA3OpW6MdA1NPRzRibHAoCHOnY8hQ3By1vnppx7RDqBq9lcJoov9qjuJWJgvkBQvxRTCbu%2BA3rW3OaAt%2FosM6uspqPMDifxOihWVjQ3muCs%2FLpjmGQ15SjxUiSY6S9r%2FoTaS7n86c7HtsTSeoLLClFyueApUybx08G4GuqOhRRnZMEilBdSlS1UnNndLCVyMF8yeMS4PkdWv8wsuScsAY6sgEeo6w45UK%2FdO35AP3P0qBiyXuUaKC5Mj6E3A7BGncf%2B02%2FiOqJCDr3YldHN2xSIIhItVmeeq4%2BUdFmRYsRlrYW1mj0%2BpyDO7n1fuhkq4V3ZRIv4PYr%2FQpxrac4dfkaacnddauaxu9U20mJg8r6BMyA40UeY1QwVIo6noPMXvuygVuroMauNLrR0%2BFCgkkd%2F2bXADUbWQrPhjMkwJwvBxXfXpaul%2BDLtFyvO2tJCK%2F%2FttTp&X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Date=20240329T222105Z&X-Amz-SignedHeaders=host&X-Amz-Expires=300&X-Amz-Credential=ASIAQ3PHCVTY2QIJUKNH%2F20240329%2Fus-east-1%2Fs3%2Faws4_request&X-Amz-Signature=2e3f2bfb6f3d511656614dd97c9462e5445a1d0a8965e31e3163d8772bce3ab9&hash=8b5dd3639bcab2c1d012b5eb6033ea080b79c9790a794e47ce58372a860103c5&host=68042c943591013ac2b2430a89b270f6af2c76d8dfd086a07176afe7c76c2c61&pii=S1877042812043923&tid=spdf-69d80448-89ac-47f6-820f-50b2ad6f93a3&sid=28b622318482f647641bd9638467e889f7a9gxrqb&type=client&tsoh=d3d3LnNjaWVuY2VkaXJlY3QuY29t&ua=140d58500c5d55575c0a&rr=86c3403d595e0551&cc=ro
